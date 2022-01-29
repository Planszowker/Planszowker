#include "Logic.h"

#include <utility>

#include "Rng/RandomGenerator.h"

namespace pla::common::games::server {

Logic::Logic(std::vector<size_t>& clientIds, const std::string& gameName)
  : m_gameName(gameName)
{
  for (auto const& clientId : clientIds) {
    m_clientsIDsAndPoints[clientId] = 0;
  }

  m_currentClientsIDAndPointsIt = m_clientsIDsAndPoints.begin();

  m_luaVM.open_libraries(sol::lib::base,
                         sol::lib::package,
                         sol::lib::io,
                         sol::lib::table,
                         sol::lib::string,
                         sol::lib::math);

  try {
    // Load State Machine and make it available as a `machine` variable in LUA
    m_luaVM.script("machine = require('lua-scripts.core.lua-state-machine')");
    // Also load JSON module
    m_luaVM.script("json = require('lua-scripts.core.lua-json')");

    // Make necessary utils visible in LUA
    auto rng = m_luaVM.new_usertype<rng::RandomGenerator>("rng",
            sol::constructors<rng::RandomGenerator(int, int)>());
    rng["generateRandomNumber"] = &rng::RandomGenerator::generateRandomNumber;

    // Bind functions to be available in LUA
    m_luaVM.set_function("AdvanceRound", &Logic::_advanceRound, this);
    m_luaVM.set_function("FinishGame", &Logic::_finishGame, this);
    m_luaVM.set_function("AddPointsToCurrentPlayer", &Logic::_addPointsToCurrentClient, this);
    m_luaVM.set_function("GetCurrentPlayerPoints", &Logic::_getCurrentPlayerPoints, this);
    m_luaVM.set_function("GetRoundsCounter", &Logic::_getRoundsCount, this);
    m_luaVM.set_function("GetCurrentPlayer", &Logic::_getCurrentClientID, this);

    // Load Game Initialization script - loading this should create states, but it's game dependent
    sol::protected_function_result result = m_luaVM.script_file(LUA_SCRIPT_GAMES_PREFIX + m_gameName + "/" + m_gameName
            + LUA_SCRIPT_GAMES_INIT_SUFFIX);
  } catch(sol::error& e) {
    std::cerr << "Exception has been raised! " << e.what() << "\n";
  }
}

bool Logic::_checkIfTurnAvailable(size_t clientId) const
{
  return m_currentClientsIDAndPointsIt->first == clientId;
}

void Logic::_advanceRound()
{
  std::cout << "Advancing round...\n";
  m_currentClientsIDAndPointsIt = std::next(m_currentClientsIDAndPointsIt);
  if (m_currentClientsIDAndPointsIt == m_clientsIDsAndPoints.end()) {
    // Roll-over -> increase round counter by 1
    m_currentClientsIDAndPointsIt = m_clientsIDsAndPoints.begin();
    ++m_roundCounter;
  }
}

void Logic::_finishGame()
{
  // Send reply to clients with players' points
  std::cout << "[CORE] Game finished.\n";
  m_finished = true;
}


void Logic::_updateClients(std::string req) const
{
  Request request {
    .type = PacketType::GameSpecificData,
    .body = std::move(req)
  };
}


void Logic::_addPointsToCurrentClient(int points)
{
  m_currentClientsIDAndPointsIt->second += points;
}


void Logic::handleGameLogic(size_t clientId, Request requestType,
                                      network::ServerPacketHandler &packetHandler)
{
  for (const auto& client: m_clientsIDsAndPoints) {
    std::cout << "Available clientID: " << client.first << "\n";
  }
  std::cout << "Current clientID turn: " << m_currentClientsIDAndPointsIt->first << "\n";

  sf::Packet replyToClient;
  Reply replyStruct {};

  // TODO: bug when previous client was deleted before game - BRD-15
  if (!_checkIfTurnAvailable(clientId)) {
    packetHandler.sendPacketToClient(clientId, replyToClient);
    return;
  }

  replyStruct.status = ReplyType::Success;

  /*try {
    m_luaVM.script("fsm:" + requestType.body + "Event()");
  } catch(sol::error& e) {
    std::cerr << "[LUA] Error: " << requestType.body + "Event does NOT exist in State Machine!\n";
  }*/

  // Request is in JSON format. It is decoded and passed as a `request` table into LUA VM.
  try {
    m_luaVM.set("request", requestType.body);
    m_luaVM.script("request = json.decode(request)");
  } catch (sol::error& e) {
    std::cerr << "[LUA] Error: Exception has been raised!\n" << e.what() << "\n";
  }

  try {
    m_luaVM.script_file(LUA_SCRIPT_GAMES_PREFIX + m_gameName + "/" + m_gameName + ".lua");
  } catch(sol::error& e) {
    std::cerr << "[LUA] Error: Exception has been raised!\n" << e.what() << "\n";
  }

  std::cout << "DEBUG: Current turn was " << m_currentClientsIDAndPointsIt->first << "\n";
}


} // namespaces
