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

namespace pla::games_server {

class ServerHandler
{
public:
  explicit ServerHandler(games::GameInstance gameInstance)
    : m_gameInstance(gameInstance)
    , m_gamesHandler(gameInstance.gameKey)
  {
  }

  virtual void run();
  virtual void stop() { m_run = false; }

protected:
  virtual bool _internalHandling();

  std::atomic<bool> m_run = true;

  games::GameInstance m_gameInstance;
  GamesHandler m_gamesHandler;

  std::map<size_t, std::shared_ptr<assets::AssetsTransmitter>> m_assetsTransmitterMap;
};

} // namespaces
