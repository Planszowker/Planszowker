#pragma once

#include <SFML/Network.hpp>
#include <unordered_map>
#include <future>
#include <mutex>
#include <atomic>
#include <functional>
#include <deque>

#include "ClientInfo/ClientInfo.h"
#include "ErrorHandler/ErrorLogger.h"

#include "PacketHandler.h"

namespace pla::network {

class SupervisorPacketHandler : public PacketHandler
{
public:
  using packetMap = std::unordered_map<size_t, std::deque<sf::Packet>>;

  explicit SupervisorPacketHandler(std::atomic_bool& run, size_t port = 0);
  virtual ~SupervisorPacketHandler();

  void runInBackground() override;

  void stop() override;

  virtual packetMap getPackets(std::vector<size_t>& keys);

  virtual void sendPacketToEveryClients(sf::Packet& packet);
  virtual void sendPacketToClient(size_t clientId, sf::Packet& packet);

  virtual inline std::atomic_bool& hasEnoughClientsConnected()
  {
    return m_hasEnoughClientsConnected;
  }

protected:
  void _backgroundTask(std::mutex& tcpSocketsMutex) override;
  virtual void _newConnectionTask(std::mutex& tcpSocketsMutex);
  virtual void _heartbeatTask(std::mutex& tcpSocketsMutex);

  virtual bool _addClient(std::shared_ptr<sf::TcpSocket>& newSocket);

  std::size_t m_maxPlayers {0}; ///< Maximum players that can join to specific instance

  sf::TcpListener m_listener; ///< TCP listener for new connections
  unsigned short m_port; ///< Current used port

  std::thread m_heartbeatThread;
  std::thread m_newConnectionThread;

  std::unordered_map<size_t, std::shared_ptr<sf::TcpSocket>> m_clients; ///< Container to hold information about clients
  std::vector<size_t> m_clientIds;

  std::size_t m_lastClientId {1}; ///< Last client ID. TODO: Possible issue with overflowing

  packetMap m_packets;

  std::atomic_bool m_hasEnoughClientsConnected {false};
};

} // namespaces
