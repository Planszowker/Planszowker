#include <Supervisor/Supervisor.h>
#include <Supervisor/GamesInfoExtractor.h>
#include <Supervisor/Lobbies.h>

#include <PlametaParser/Entry.h>
#include <Games/Objects.h>

#include <string>
#include <iostream>
#include <thread>

namespace pla::supervisor {

using namespace games;

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

  // Do something...
  while(m_run) {
    _processPackets(supervisorPacketHandler);
  }

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
        Reply idReply{
                .type = PacketType::ID,
                .status = ReplyType::Success,
                .body = std::to_string(clientIdKey)
        };
        sf::Packet replyPacket;
        replyPacket << idReply;

        packetHandler.sendPacketToClient(clientIdKey, replyPacket);
        continue;
      } else if (request.type == PacketType::ListAvailableGames) {
        _listAvailableGamesHandler(clientIdKey, packetHandler);
        continue;
      } else if (request.type == PacketType::CreateLobby) {
        _createLobbyHandler(clientIdKey, packetHandler, nlohmann::json::parse(request.body));
        continue;
      }

      std::cout << "Type: " << static_cast<int>(request.type) << "\n";
      std::cout << "Body: " << request.body << "\n";
    }
  }
}


void Supervisor::_listAvailableGamesHandler(size_t clientIdKey, network::SupervisorPacketHandler& packetHandler)
{
  GamesInfoExtractor gamesInfoExtractor;

  auto& entries = gamesInfoExtractor.getEntries();
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

  Lobbies::createNewLobby(clientIdKey, requestJson.at("LobbyName"), requestJson.at("GameKey"));

  sf::Packet packet;
  packet << reply;

  packetHandler.sendPacketToClient(clientIdKey, packet);
}

} // namespace
