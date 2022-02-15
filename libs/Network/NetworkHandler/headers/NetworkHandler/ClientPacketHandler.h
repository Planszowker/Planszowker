#pragma once

#include <SFML/Network.hpp>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <memory>
#include <unordered_map>
#include <deque>

#include <Games/Objects.h>

#include "PacketHandler.h"

namespace pla::common::network {

enum class TransactionState : uint8_t {
  NotStarted,
  InProgress,
};

class ClientPacketHandler : public PacketHandler
{
public:

  explicit ClientPacketHandler(sf::TcpSocket& serverSocket);
  ~ClientPacketHandler();

  void runInBackground() final;
  void stop() final;

  bool sendPacket(sf::Packet& packet);
  std::deque<games::Reply> getReplies();

  /**
   * Get raw packets from assets transfer
   * @return Deque with assets chunks
   */
  std::deque<sf::Packet> getRawPackets();

  bool getClientID(size_t& id);

private:

  void _backgroundTask(std::mutex& tcpSocketsMutex) final;

  // Connection related variables
  sf::TcpSocket& m_serverSocket;
  std::deque<games::Reply> m_receivedReplies;
  std::deque<sf::Packet> m_receivedRawPackets;

  bool m_validID{false};
  size_t m_clientID{0};

  TransactionState m_transactionState;
  std::string m_currentAssetKey;
};

} // namespaces
