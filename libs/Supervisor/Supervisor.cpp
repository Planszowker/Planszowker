#include <Supervisor/Supervisor.h>
#include <Supervisor/GamesInfoExtractor.h>
#include <Supervisor/Lobbies.h>

#include <PlametaParser/Entry.h>
#include <Games/Objects.h>

#include <nlohmann/json.hpp>

#include <string>
#include <iostream>
#include <thread>

namespace pla::supervisor {

using namespace games;
using namespace games::json_entries;

Supervisor::Supervisor(std::stringstream configStream)
  : m_configParser(std::move(configStream))
{
  auto helpCmd = std::make_shared<Command>(
          "help",
          "Lists all available commands",
          [this]()
          {
            std::cout << "Available commands:\n";
            for (const auto& command : this->m_commands) {
              std::cout << "\t" << command->getCommand() << " - " << command->getDescription() << "\n";
            }
          }
  );

  auto quitCmd = std::make_shared<Command>(
          "quit",
          "Stops server and all game instances",
          [this]()
          {
            this->m_run = false;
          }
  );

  _registerCommand(std::move(helpCmd));
  _registerCommand(std::move(quitCmd));
}


void Supervisor::run()
{
  auto entryPtr = m_configParser["config:port"];
  std::cout << "[Config]:port = " << std::get<int>(entryPtr->getVariant()) << "\n";

  std::size_t port = static_cast<size_t>(std::get<int>(entryPtr->getVariant()));
  network::SupervisorPacketHandler supervisorPacketHandler {m_run, port};

  supervisorPacketHandler.runInBackground();
  std::thread inputThread {&Supervisor::_getUserInput, this};

  Lobbies::startWatchdogThread(supervisorPacketHandler);

  while(m_run) {
    _processPackets(supervisorPacketHandler);
  }

  Lobbies::stopWatchdogThread();

  inputThread.join();
}


void Supervisor::_registerCommand(std::shared_ptr<Command>&& command)
{
  m_commands.push_back(std::move(command));
}


void Supervisor::_getUserInput()
{
  std::string inputCommand;

  std::cout << "Planszówker Server v" << PROJECT_VER << "\n"
                                                        "(Type \'help\' for list of all available commands)\n"
                                                        "================================================\n";

  while (m_run) {
    std::cout << "Command: ";

    std::cin >> inputCommand;

    bool cmdFound {false};
    for (const auto& command : m_commands) {
      if (command->getCommand().compare(inputCommand) == 0) {
        command->invoke();
        cmdFound = true;
        break;
      }
    }

    if (!cmdFound) {
      std::cout << "Command not found!\n";
    }
  }
}


void Supervisor::_processPackets(network::SupervisorPacketHandler& packetHandler)
{
  // TODO: Refactor this bulk...
  std::vector<size_t> clientIdKeys;
  auto packetsMap = packetHandler.getPackets(clientIdKeys);

  for (const auto& clientIdKey: clientIdKeys) {
    // Find map entry with client ID -> (size_t, deque) is received
    const auto mapIt = packetsMap.find(clientIdKey);
    if (mapIt == packetsMap.end()) {
      continue;
    }

    // Iterate over all packets in deque
    for (auto& packet: mapIt->second) {
      Request request{};
      packet >> request;

      if (request.type == PacketType::Heartbeat) {
        // We don't care about Heartbeat packets
        continue;
      } else if (request.type == PacketType::ID) {
        // If we get ID request, we need to send client's ID
        nlohmann::json replyJson;
        replyJson[CLIENT_ID] = clientIdKey;

        Reply idReply{
                .type = PacketType::ID,
                .status = ReplyType::Success,
                .body = replyJson.dump()
        };

        sf::Packet replyPacket;
        replyPacket << idReply;

        packetHandler.sendPacketToClient(clientIdKey, replyPacket);
      } else if (request.type == PacketType::ListAvailableGames) {
        Supervisor::_listAvailableGamesHandler(clientIdKey, packetHandler);
      } else if (request.type == PacketType::CreateLobby) {
        Supervisor::_createLobbyHandler(clientIdKey, packetHandler, nlohmann::json::parse(request.body));
      } else if (request.type == PacketType::GetLobbyDetails) {
        Supervisor::_getLobbyDetailsHandler(clientIdKey, packetHandler, nlohmann::json::parse(request.body));
      } else if (request.type == PacketType::ListOpenLobbies) {
        Supervisor::_listOpenLobbiesHandler(clientIdKey, packetHandler, nlohmann::json::parse(request.body));
      } else if (request.type == PacketType::JoinLobby) {
        Supervisor::_joinLobbyHandler(clientIdKey, packetHandler, nlohmann::json::parse(request.body));
      } else if (request.type == PacketType::LobbyHeartbeat) {
        Supervisor::_lobbyHeartbeatHandler(clientIdKey, packetHandler, nlohmann::json::parse(request.body));
      }

      std::cout << "Type: " << static_cast<int>(request.type) << "\n";
      try {
        std::cout << "Request JSON:\n" << nlohmann::json::parse(request.body).dump(4) << "\n";
      } catch (std::exception& e) {
        std::cout << "Request:" << request.body << "\n";
      }
    }
  }
}


void Supervisor::_listAvailableGamesHandler(size_t clientIdKey, network::SupervisorPacketHandler& packetHandler)
{
  // Remove a lobby from a list if exists
  Lobbies::removeLobby(clientIdKey);

  static GamesInfoExtractor gamesInfoExtractor;

  auto& metaAssets = gamesInfoExtractor.getMetaAssets();

  for(const auto& metaAsset: metaAssets) {
    Reply reply {.type = games::PacketType::ListAvailableGames, .status = games::ReplyType::Success};
    reply.body = metaAsset.first + "::" + metaAsset.second;

    LOG(DEBUG) << "Reply length: " << reply.body.length();

    sf::Packet packet;
    packet << reply;

    packetHandler.sendPacketToClient(clientIdKey, packet);
  }
}


void Supervisor::_createLobbyHandler(size_t clientIdKey, network::SupervisorPacketHandler& packetHandler, const nlohmann::json& requestJson)
{
  Reply reply {
    .type = games::PacketType::CreateLobby,
    .status = games::ReplyType::Success,
  };

  LOG(DEBUG) << "[Create Lobby Handler]";
  try {
    auto lobby = Lobbies::createNewLobby(clientIdKey, requestJson.at(LOBBY_NAME), requestJson.at(GAME_KEY));

    sf::Packet packet;

    nlohmann::json replyJson;
    replyJson[VALID] = true;
    reply.body = replyJson.dump();

    packet << reply;

    packetHandler.sendPacketToClient(clientIdKey, packet);

    lobby->sendUpdate(packetHandler);
  } catch (std::exception& e) { }
}


void Supervisor::_getLobbyDetailsHandler(size_t clientIdKey, network::SupervisorPacketHandler &packetHandler, const nlohmann::json &requestJson)
{
  Reply reply {
    .type = games::PacketType::GetLobbyDetails,
    .status = games::ReplyType::Success,
  };

  LOG(DEBUG) << "[Get Lobby Details Handler]";

  auto lobby = Lobbies::getLobby(requestJson[CREATOR_ID]);
  if (lobby) {
    LOG(DEBUG) << "[LobbyDetailsHandler] CreatorID: " << lobby->getCreatorClientId();
    LOG(DEBUG) << "[LobbyDetailsHandler] Lobby Name: " << lobby->getLobbyName();
    LOG(DEBUG) << "[LobbyDetailsHandler] Game Key: " << lobby->getGameKey();

    nlohmann::json replyJson;
    replyJson[CREATOR_ID] = lobby->getCreatorClientId();
    replyJson[CLIENT_IDS] = lobby->getClients();
    replyJson[LOBBY_NAME] = lobby->getLobbyName();
    replyJson[GAME_KEY] = lobby->getGameKey();
    replyJson[MAX_PLAYERS] = lobby->getMaxPlayers();
    replyJson[MIN_PLAYERS] = lobby->getMinPlayers();
    replyJson[CURRENT_PLAYERS] = lobby->getCurrentPlayers();
    replyJson[VALID] = true;
    reply.body = replyJson.dump();

    sf::Packet packet;
    packet << reply;

    packetHandler.sendPacketToClient(clientIdKey, packet);
  }
}


void Supervisor::_listOpenLobbiesHandler(size_t clientIdKey, network::SupervisorPacketHandler& packetHandler, const nlohmann::json& requestJson)
{
  Reply reply {
    .type = games::PacketType::ListOpenLobbies,
    .status = games::ReplyType::Success,
  };

  LOG(DEBUG) << "[List Open Lobbies Handler]";

  // Remove a lobby from a list if exists
  Lobbies::removeLobby(clientIdKey);

  try {
    auto gameKey = requestJson[GAME_KEY].get<std::string>();

    nlohmann::json replyJson;
    replyJson[GAME_KEY] = gameKey;
    replyJson[LOBBIES] = nlohmann::json::array();
    for (const auto& [creatorID, lobby]: Lobbies::getLobbies()) {
      // Only look for lobbies with given game key
      if (lobby.getGameKey() == gameKey) {
        replyJson[LOBBIES].push_back({
          {CREATOR_ID, lobby.getCreatorClientId()},
          {LOBBY_NAME, lobby.getLobbyName()},
          {MIN_PLAYERS, lobby.getMinPlayers()},
          {CURRENT_PLAYERS, lobby.getCurrentPlayers()},
          {MAX_PLAYERS, lobby.getMaxPlayers()}
        });
      }
    }
    replyJson[VALID] = true;
    reply.body = replyJson.dump();

    LOG(DEBUG) << "Reply:\n" << replyJson.dump(4);

    sf::Packet packet;
    packet << reply;

    packetHandler.sendPacketToClient(clientIdKey, packet);
  } catch (const std::exception& e) {
    LOG(DEBUG) << "[ListOpenLobbiesHandler] Exception!";
  }
}


void Supervisor::_joinLobbyHandler(size_t clientIdKey, network::SupervisorPacketHandler& packetHandler, const nlohmann::json& requestJson)
{
  Reply reply {
    .type = games::PacketType::JoinLobby,
    .status = games::ReplyType::Success,
  };

  LOG(DEBUG) << "[Join Lobby Handler]";

  auto lobby = Lobbies::getLobby(requestJson[CREATOR_ID].get<size_t>());
  if (lobby and requestJson[CREATOR_ID].get<size_t>() != clientIdKey) {
    nlohmann::json replyJson;
    replyJson[VALID] = false;

    if (lobby->addClient(clientIdKey)) {
      replyJson[VALID] = true;
    }

    reply.body = replyJson.dump();

    sf::Packet packet;
    packet << reply;

    packetHandler.sendPacketToClient(clientIdKey, packet);

    lobby->sendUpdate(packetHandler);
  }
}


void Supervisor::_lobbyHeartbeatHandler(size_t clientIdKey, network::SupervisorPacketHandler& packetHandler, const nlohmann::json& requestJson)
{
  try {
    auto type = requestJson[LOBBY_HEARTBEAT_TYPE].get<std::string>();
    if (type == "Creator") {
      // Update last response time for lobby
      Lobbies::updateLobbyLastResponseTime(clientIdKey);
    } else if (type == "Client") {
      auto creatorId = requestJson[CREATOR_ID].get<size_t>();
      Lobbies::updateClientLastResponseTime(creatorId, clientIdKey);
    }
  } catch (std::exception& e) {
  }
}

} // namespace
