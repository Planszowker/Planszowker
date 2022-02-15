#include "ClientPacketHandler.h"

#include "Games/Objects.h"
#include "TimeMeasurement/TimeLogger.h"
#include "CompilerUtils/FunctionInfoExtractor.h"
#include <AssetsManager/AssetsReceiver.h>

#include <chrono>
#include <sstream>

#include <iostream> // TODO: remove debug

using namespace pla::common::time_measurement;

namespace pla::common::network {

ClientPacketHandler::ClientPacketHandler(sf::TcpSocket& serverSocket)
  : PacketHandler()
  , m_serverSocket(serverSocket)
  , m_transactionState(TransactionState::NotStarted)
{
  m_serverSocket.setBlocking(false);
}


ClientPacketHandler::~ClientPacketHandler() {
  m_backgroundThread.join();
}


void ClientPacketHandler::runInBackground()
{
  m_run = true;

  std::thread backgroundThread{&ClientPacketHandler::_backgroundTask, this, std::ref(m_tcpSocketsMutex)};
  m_backgroundThread = std::move(backgroundThread);
}


void ClientPacketHandler::stop() {
  m_run = false;
}


void ClientPacketHandler::_backgroundTask(std::mutex& tcpSocketsMutex)
{
  /*
   * This method will receive and send packets to server
   */
  while(m_run) {
    std::this_thread::sleep_for(std::chrono::milliseconds (10));

    TimeLogger logger(GET_CURRENT_FUNCTION_NAME());
    const std::scoped_lock tcpSocketsLock(tcpSocketsMutex);

    // Receive packets from server.
    sf::Packet receivePacket;
    sf::Socket::Status socketStatus = m_serverSocket.receive(receivePacket);
    if (socketStatus == sf::Socket::NotReady) {
      continue;
    }

    if (socketStatus == sf::Socket::Done) {
      games::Reply reply;

      if (!(receivePacket >> reply)) {
        // If we fail to extract reply from packet, it means the packet is either corrupted
        // or is a part of transaction.
        if (m_transactionState == TransactionState::InProgress) {
          // If it's a transaction part, add it to deque and stop processing.
          m_receivedRawPackets.push_back(receivePacket);
          continue;
        }

        // If it's a corrupted packet, just continue.
        continue;
      }

      // If received status is not Success, we shouldn't handle it.
      if (reply.status != games::ReplyType::Success) {
        continue;
      }

      // Handle other packets type.
      switch (reply.type) {
        case games::PacketType::GameSpecificData:
        {
          m_receivedReplies.push_back(reply);
          break;
        }

        case games::PacketType::ID:
        {
          m_validID = true;
          std::stringstream ss{reply.body};
          ss >> m_clientID;
          std::cout << "\nReceived ClientID: " << m_clientID << "\n";
          break;
        }

        case games::PacketType::StartTransaction:
        {
          if (m_transactionState == TransactionState::NotStarted) {
            std::cout << "[DEBUG] Started transaction for " << reply.body << "!\n";
            m_currentAssetKey = reply.body;
            m_transactionState = TransactionState::InProgress;
            m_receivedRawPackets.clear();
          }
          break;
        }

        case games::PacketType::EndTransaction:
        {
          if (m_transactionState == TransactionState::InProgress) {
            std::cout << "[DEBUG] Ended transaction for " << reply.body << "!\n";

            if (!assets::AssetsReceiver::parseAndAddAsset(m_receivedRawPackets, m_currentAssetKey)) {
              std::cerr << "Error adding " << m_currentAssetKey << "!\n";
            }

            m_transactionState = TransactionState::NotStarted;
          }
          break;
        }

        default:
          break;
      }
    }
  }
}

bool ClientPacketHandler::sendPacket(sf::Packet& packet) {
  TimeLogger logger(GET_CURRENT_FUNCTION_NAME());
  // Obtain mutex
  const std::scoped_lock tcpSocketsLock(m_tcpSocketsMutex);

  // Send packet to server
  sf::Socket::Status retStatus = m_serverSocket.send(packet);
  while (retStatus == sf::Socket::Partial) {
    retStatus = m_serverSocket.send(packet);
  }

  return (retStatus == sf::Socket::Done);
}

std::deque<games::Reply> ClientPacketHandler::getReplies() {
  TimeLogger logger(GET_CURRENT_FUNCTION_NAME());
  const std::scoped_lock tcpSocketsLock(m_tcpSocketsMutex);

  std::deque<games::Reply> returnDeque = m_receivedReplies;
  m_receivedReplies.clear();

  return std::move(returnDeque);
}


bool ClientPacketHandler::getClientID(size_t& id) {
  const std::scoped_lock tcpSocketsLock(m_tcpSocketsMutex); // TODO: Check if needed

  id = m_clientID;

  return m_validID;
}

} // namespaces
