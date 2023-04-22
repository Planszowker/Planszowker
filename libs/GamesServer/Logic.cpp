#include "Logic.h"

#include <fstream>
#include <utility>

#include <Rng/RandomGenerator.h>
#include <GamesHandler.h>

#include <easylogging++.h>
#include <nlohmann/json.hpp>

namespace pla::games_server {

using namespace games;
using namespace games::json_entries;

Logic::Logic(std::vector<size_t>& clientIds, const std::string& gameName, network::SupervisorPacketHandler& packetHandler, ZipArchive::Ptr zipFile)
  : m_gameName(gameName)
  , m_clientsIDs(clientIds)
  , m_networkHandler(packetHandler)
  , m_plaGameFile(std::move(zipFile))
{
  for (auto const& clientId : clientIds) {
    m_clientsIDsAndPoints[clientId] = 0;
  }

  m_currentClientsIDAndPointsIt = m_clientsIDsAndPoints.begin();

  m_luaVM.open_libraries(sol::lib::base,
                         sol::lib::package,
                         sol::lib::table,
                         sol::lib::string,
                         sol::lib::math);

  try {
    // Create entries from the content of .plagame
    std::string gameDir = m_gameName + '/';
    m_boardEntry = m_plaGameFile->GetEntry(gameDir + GamesHandler::BOARD_DESCRIPTION_FILE);
    m_gameEntry = m_plaGameFile->GetEntry((gameDir + m_gameName + GamesHandler::LUA_SCRIPT_EXTENSION));
    m_initEntry = m_plaGameFile->GetEntry((gameDir + m_gameName + GamesHandler::LUA_SCRIPT_INIT_SUFFIX));

    // Read content of files inside .plagame file into string streams
    m_boardScript << m_boardEntry->GetDecompressionStream()->rdbuf();
    m_initScript << m_initEntry->GetDecompressionStream()->rdbuf();
    m_gameScript << m_gameEntry->GetDecompressionStream()->rdbuf();

    // Close streams
    m_boardEntry->CloseDecompressionStream();
    m_initEntry->CloseDecompressionStream();
    m_gameEntry->CloseDecompressionStream();

    m_luaVM["BoardDescriptionString"] = m_boardScript.str();

    // Load core LUA modules
    m_luaVM.script("Machine = require('scripts.core.lua-state-machine')"); // State machine
    m_luaVM.script("Json = require('scripts.core.lua-json')"); // JSON encode-decode
    m_luaVM.script("ReplyModule = require('scripts.core.lua-reply')"); // Reply Module
    m_luaVM.script("ActionRequest = require('scripts.core.lua-action-request')"); // ActionRequest Module
    m_luaVM.script("Helper = require('scripts.core.lua-helper')"); // Helper Module

    // Load Game Objects
    m_luaVM.script("Entity = require('scripts.core.objects.entity')");
    m_luaVM.script("DestinationPoint = require('scripts.core.objects.destination-point')");
    m_luaVM.script("GameObjects = require('scripts.core.lua-game-objects')"); // Game Objects

    // Make necessary utils visible in LUA
    auto rng = m_luaVM.new_usertype<rng::RandomGenerator>("Rng",
            sol::constructors<rng::RandomGenerator(int, int)>());
    rng["GenerateRandomNumber"] = &rng::RandomGenerator::generateRandomNumber;

    // Bind functions to be available in LUA
    m_luaVM.set_function("AdvanceRound", &Logic::_advanceRound, this);
    m_luaVM.set_function("FinishGame", &Logic::_finishGame, this);
    m_luaVM.set_function("AddPointsToCurrentPlayer", &Logic::_addPointsToCurrentClient, this);
    m_luaVM.set_function("GetCurrentPlayerPoints", &Logic::_getCurrentPlayerPoints, this);
    m_luaVM.set_function("GetRoundsCounter", &Logic::_getRoundsCount, this);
    m_luaVM.set_function("GetCurrentPlayer", &Logic::_getCurrentClientID, this);
    m_luaVM.set_function("GetPlayers", &Logic::_getClients, this);
    m_luaVM.set_function("GetPlayerPoints", &Logic::_getClientPoints, this);
    m_luaVM.set_function("SendReply", &Logic::_updateClients, this);

    // Invoke init script from .plagame file
    m_luaVM.script(m_initScript.str());
  } catch(sol::error& e) {
    LOG(ERROR) << "Exception has been raised! " << e.what();
  }
}

bool Logic::_checkIfTurnAvailable(size_t clientId) const
{
  return m_currentClientsIDAndPointsIt->first == clientId;
}

void Logic::_advanceRound()
{
  LOG(DEBUG) << "Advancing round...";
  m_currentClientsIDAndPointsIt = std::next(m_currentClientsIDAndPointsIt);
  if (m_currentClientsIDAndPointsIt == m_clientsIDsAndPoints.end()) {
    // Roll-over -> increase round counter by 1
    m_currentClientsIDAndPointsIt = m_clientsIDsAndPoints.begin();
    ++m_roundCounter;
  }
}

void Logic::_finishGame()
{
  LOG(DEBUG) << "[CORE] Game finished.";
  m_finished = true;
}


void Logic::_updateClients(std::string req) const
{
  Reply reply {
    .type = PacketType::GameSpecificData,
    .body = std::move(req)
  };

  sf::Packet replyPacket;
  replyPacket << reply;

  for (auto clientId : m_clientsIDs) {
    m_networkHandler.sendPacketToClient(clientId, replyPacket);
  }
}


int Logic::_getClientPoints(size_t clientID) const
{
  int retValue {0};
  auto foundIt = m_clientsIDsAndPoints.find(clientID);
  if (foundIt != m_clientsIDsAndPoints.end()) {
    // We found a client's entry
    retValue = foundIt->second;
  }

  return retValue;
}


void Logic::_addPointsToCurrentClient(int points)
{
  m_currentClientsIDAndPointsIt->second += points;
}


void Logic::handleGameLogic(size_t clientId, const Request& requestType)
{
  for (const auto& client: m_clientsIDsAndPoints) {
    LOG(DEBUG) << "Available clientID: " << client.first;
  }
  LOG(DEBUG) << "Current clientID turn: " << m_currentClientsIDAndPointsIt->first;

  // Check if turn is available for given client
  if (!_checkIfTurnAvailable(clientId)) {
    sf::Packet packet;
    nlohmann::json replyJson;
    replyJson[VALID] = false;

    Reply reply {
      .type = games::PacketType::GameSpecificData,
      .body = replyJson.dump(),
    };
    packet << reply;

    m_networkHandler.sendPacketToClient(clientId, packet);
    return;
  }

  // Every request handling is divided into 3 sections:
  //   - request decoding using JSON decoder and pre-processing,
  //   - invoking <GameName>.lua script,
  //   - sending reply to clients and post-processing.

  // Request is in JSON format. It is decoded and passed as a `request` table into LUA VM.
  try {
    m_luaVM.set("Request", requestType.body);
    m_luaVM.script("Request = Json.decode(Request)");
  } catch (sol::error& e) {
    LOG(ERROR) << "[LUA] Error: Exception has been raised!\n" << e.what();
  }

  // Create `Reply` table
  m_luaVM["Reply"] = m_luaVM.create_table();

  // Invoking <GameName>.lua script
  try {
    m_luaVM.script(m_gameScript.str());
  } catch(sol::error& e) {
    LOG(ERROR) << "[LUA] Error: Exception has been raised!\n" << e.what();
  }

  // Sending Reply to Clients
  try {
    m_luaVM["Reply"]["GameFinished"] = m_finished;
    m_luaVM.script("ReplyModule:SendReply()");
  } catch (sol::error& e) {
    LOG(ERROR) << "[LUA] Error: Exception has been raised!\n" << e.what();
  }
}


} // namespaces
