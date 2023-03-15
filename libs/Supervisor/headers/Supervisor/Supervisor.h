#pragma once

#include <Supervisor/GamesInfoExtractor.h>
#include <Supervisor/Command.h>

#include <AssetsManager/AssetsTransmitter.h>
#include <NetworkHandler/SupervisorPacketHandler.h>
#include <PlametaParser/Parser.h>
#include <ThreadSafeQueue/ThreadSafeQueue.h>
#include <Games/Objects.h>
#include <Games/GameInstance.h>
#include <GamesServer/ServerHandler.h>
#include <Supervisor/Lobby.h>

#include <easylogging++.h>
#include <nlohmann/json.hpp>

#include <atomic>
#include <memory>
#include <sstream>
#include <tuple>

namespace pla::games_server {
class ServerHandler;
}

namespace pla::supervisor {

class Supervisor
{
public:
  Supervisor() = delete;
  explicit Supervisor(std::stringstream configStream);

  void run();

private:
  using GameInstancesTuple = std::tuple<std::shared_ptr<games_server::ServerHandler>, games::GameInstanceSyncParameters>;

  void _getUserInput();
  void _registerCommand(std::shared_ptr<Command>&& command);
  void _processPackets(network::SupervisorPacketHandler& packetHandler);

  static void _listAvailableGamesHandler(size_t clientIdKey, network::SupervisorPacketHandler& packetHandler);
  static void _createLobbyHandler(size_t clientIdKey, network::SupervisorPacketHandler& packetHandler, const nlohmann::json& requestJson);
  static void _getLobbyDetailsHandler(size_t clientIdKey, network::SupervisorPacketHandler& packetHandler, const nlohmann::json& requestJson);
  static void _listOpenLobbiesHandler(size_t clientIdKey, network::SupervisorPacketHandler& packetHandler, const nlohmann::json& requestJson);
  static void _joinLobbyHandler(size_t clientIdKey, network::SupervisorPacketHandler& packetHandler, const nlohmann::json& requestJson);
  static void _lobbyHeartbeatHandler(size_t clientIdKey, network::SupervisorPacketHandler& packetHandler, const nlohmann::json& requestJson);
  static void _startGameHandler(size_t clientIdKey, network::SupervisorPacketHandler& packetHandler);

  static void _createNewGameInstance(network::SupervisorPacketHandler& packetHandler, const Lobby& lobby);

  utils::plameta::Parser m_configParser;

  std::atomic_bool m_run {true};

  std::vector<std::shared_ptr<Command>> m_commands;

  std::map<size_t, std::shared_ptr<assets::AssetsTransmitter>> m_assetsTransmitterMap;

  static std::unordered_map<size_t, GameInstancesTuple> m_gameInstances;
};

}