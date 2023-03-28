#pragma once

#include <SFML/Network.hpp>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <memory>
#include <unordered_map>
#include <deque>

#include <Games/CommObjects.h>
#include <Games/Callbacks/ICallbacks.h>

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
  virtual ~ClientPacketHandler();

  void runInBackground() final;
  void stop() final;

  bool sendPacket(sf::Packet& packet);
  std::deque<games::Reply> getReplies();

  void connectCallbacks(games::ICallbacks* callbacks);

private:

  void _backgroundTask() final;

  bool _requestAsset();

  // Connection related variables
  sf::TcpSocket& m_serverSocket;
  std::deque<games::Reply> m_receivedReplies;
  std::deque<sf::Packet> m_receivedRawPackets;

  TransactionState m_transactionState;
  std::string m_currentAssetKey;

  size_t m_transactionCounter {0};

  games::ICallbacks* m_callbacks;
};

} // namespaces
