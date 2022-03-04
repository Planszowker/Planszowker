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

namespace pla::network {

enum class TransactionState : uint8_t {
  NotStarted,
  InProgress,
};

class ClientPacketHandler : public PacketHandler
{
public:

  explicit ClientPacketHandler(std::atomic_bool& run, sf::TcpSocket& serverSocket);
  ~ClientPacketHandler();

  void runInBackground() final;
  void stop() final;

  bool sendPacket(sf::Packet& packet);
  std::deque<games::Reply> getReplies();

  bool getClientID(size_t& id);

private:

  void _backgroundTask(std::mutex& tcpSocketsMutex) final;

  bool _requestAsset();
  void _listAllAvailableGames();

  // Connection related variables
  sf::TcpSocket& m_serverSocket;
  std::deque<games::Reply> m_receivedReplies;
  std::deque<sf::Packet> m_receivedRawPackets;

  bool m_validID{false};
  size_t m_clientID{0};

  TransactionState m_transactionState;
  std::string m_currentAssetKey;

  size_t m_transactionCounter {0};
};

} // namespaces
