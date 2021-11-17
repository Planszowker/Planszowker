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

namespace pla::common::network {

class ServerPacketHandler : public PacketHandler
{
public:
  using packetMap = std::unordered_map<size_t, std::deque<sf::Packet>>;


  /*!
  * @brief Constructor with maximum number of players connected at the same time
  */
  explicit ServerPacketHandler(size_t maxPlayers = 0);
  ~ServerPacketHandler();

  void runInBackground() final;

  void stop() final;

  inline std::atomic_bool& hasEnoughClientsConnected()
  {
    return m_hasEnoughClientsConnected;
  }

  packetMap& getPackets(std::vector<size_t>& keys);
  void clearPacketsForClient(size_t clientId);

  void sendPacketToEveryClients(sf::Packet& packet);
  void sendPacketToClient(size_t clientId, sf::Packet& packet);

private:

  void _backgroundTask(std::mutex& tcpSocketsMutex) final;
  void _newConnectionTask(std::mutex& tcpSocketsMutex);
  void _heartbeatTask(std::mutex& tcpSocketsMutex);

  bool _addClient(std::shared_ptr<sf::TcpSocket>& newSocket);

  std::size_t m_maxPlayers; ///< Maximum players that can join to specific instance

  sf::TcpListener m_listener; ///< TCP listener for new connections
  unsigned short m_port; ///< Current used port

  std::thread m_heartbeatThread;
  std::thread m_newConnectionThread;

  std::unordered_map<size_t, std::shared_ptr<sf::TcpSocket>> m_clients; ///< Container to hold information about clients
  std::vector<size_t> m_clientIds;

  std::size_t m_lastClientId; ///< Last client ID. TODO: Possible issue with overflowing

  packetMap m_packets;

  std::atomic_bool m_hasEnoughClientsConnected;
};

} // namespaces
