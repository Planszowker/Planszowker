#pragma once

/* Generic */
#include "NetworkHandler/ServerPacketHandler.h"

/* SFML */
#include <SFML/Network.hpp>

/* STD */
#include <vector>
#include <memory>
#include <atomic>

namespace pla::common::games {

class ServerHandler
{
public:
  explicit ServerHandler(network::ServerPacketHandler& networkHandler)
    : m_networkHandler(networkHandler)
    , m_running(true)
  {
  }

  virtual void run() = 0;
  virtual void stop() = 0;

  virtual void networkCall(std::shared_ptr<sf::TcpSocket>& client, sf::Packet& packet, size_t playerId) = 0;

protected:
  std::atomic_bool m_running;
  network::ServerPacketHandler& m_networkHandler;
};

} // namespaces
