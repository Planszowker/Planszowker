#pragma once

#include "NetworkHandler/NetworkHandler.h"
#include "Games/ServerLogic.h"

#include <SFML/Network.hpp>

#include <vector>
#include <memory>
#include <atomic>

namespace pla::common::games {

class ServerHandler
{
public:
  virtual void run() = 0;
  virtual void stop() = 0;

  virtual void networkCall(std::shared_ptr<sf::TcpSocket>& client, sf::Packet& packet, size_t playerId) = 0;
  virtual void setPlayersIds(std::vector<size_t> playerIds) = 0;

protected:
  std::atomic_bool m_running;
  std::unique_ptr<network::NetworkHandler> m_networkHandler;
  std::unique_ptr<ServerLogic> m_logic;
};

} // namespaces
