#include "Command.h"
#include "GamesInfoExtractor.h"

#include <AssetsManager/AssetsTransmitter.h>
#include <NetworkHandler/SupervisorPacketHandler.h>
#include <PlametaParser/Parser.h>
#include <easylogging++.h>
#include <nlohmann/json.hpp>

#include <atomic>
#include <memory>
#include <sstream>

namespace pla::supervisor {

class Supervisor
{
public:
  Supervisor() = delete;
  explicit Supervisor(std::stringstream configStream);

  void run();

private:
  void _getUserInput();
  void _registerCommand(std::shared_ptr<Command>&& command);
  void _processPackets(network::SupervisorPacketHandler& packetHandler);

  void _listAvailableGamesHandler(size_t clientIdKey, network::SupervisorPacketHandler& packetHandler);
  void _createLobbyHandler(size_t clientIdKey, network::SupervisorPacketHandler& packetHandler, const nlohmann::json& requestJson);

  utils::plameta::Parser m_configParser;

  std::atomic_bool m_run {true};

  std::vector<std::shared_ptr<Command>> m_commands;

  std::map<size_t, std::shared_ptr<assets::AssetsTransmitter>> m_assetsTransmitterMap;
};

}