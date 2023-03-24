#pragma once

#include <GamesServer/GamesHandler.h>

/* Generic */
#include <NetworkHandler/SupervisorPacketHandler.h>
#include <AssetsManager/AssetsTransmitter.h>
#include <Supervisor/Supervisor.h>
#include <Games/GameInstance.h>

/* SFML */
#include <SFML/Network.hpp>

/* STD */
#include <vector>
#include <memory>
#include <atomic>
#include <unordered_map>

namespace pla::games_server {

class ServerHandler
{
public:
  explicit ServerHandler(games::GameInstance gameInstance)
    : m_gameInstance(gameInstance)
    , m_gamesHandler(gameInstance.gameKey)
  {
  }

  void run();
  void stop();

  void transmitAssetsToClient(size_t clientId);

protected:
  virtual bool _internalHandling();

  std::atomic<bool> m_run = true;
  std::mutex m_mutex; ///< Some methods may be accessed from Supervisor, thus we need to protect resources

  games::GameInstance m_gameInstance;
  GamesHandler m_gamesHandler;

  std::unordered_map<size_t, std::shared_ptr<assets::AssetsTransmitter>> m_assetsTransmitterMap;
};

} // namespaces
