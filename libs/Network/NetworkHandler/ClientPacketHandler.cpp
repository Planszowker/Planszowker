#include "ClientPacketHandler.h"

#include <Games/Objects.h>
#include <TimeMeasurement/TimeLogger.h>
#include <CompilerUtils/FunctionInfoExtractor.h>
#include <AssetsManager/AssetsReceiver.h>
#include <AssetsManager/AssetsDefines.h>
#include <ErrorHandler/ErrorLogger.h>

#include <chrono>
#include <any>

#include <nlohmann/json.hpp>
#include <easylogging++.h>

using namespace pla::time_measurement;

namespace pla::network {

ClientPacketHandler::ClientPacketHandler(std::atomic_bool& run, sf::TcpSocket& serverSocket)
  : PacketHandler(run)
  , m_serverSocket(serverSocket)
  , m_transactionState(TransactionState::NotStarted)
  , m_callbacks(nullptr)
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
  // This method will receive and send packets to server
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
        // or is a part of asset transaction.
        if (m_transactionState == TransactionState::InProgress) {
          // If it's a transaction part, add it to deque and stop processing.
          m_receivedRawPackets.push_back(receivePacket);
          ++m_transactionCounter;

          //LOG(DEBUG) << "[DEBUG] Transaction counter: " << m_transactionCounter << "\n";

          if (m_transactionCounter == assets::CHUNK_QUANTITY) {
            // We have received enough chunks, we need to send a request for more data.
            // Cannot use SendPacket(), because it would require double mutex locking...
            //LOG(DEBUG) << "[DEBUG] Requesting asset...\n";
            m_transactionCounter = 0;
            _requestAsset();
          }

          continue;
        }

        // If it's a corrupted packet, just continue.
        std::cerr << "[DEBUG] Corrupted packet!\n";
        continue;
      }

      // If received status is not Success, we shouldn't handle it.
      if (reply.status != games::ReplyType::Success || reply.type == games::PacketType::Heartbeat) {
        continue;
      }

      try {
        LOG(DEBUG) << "Reply:\n" << nlohmann::json::parse(reply.body).dump(4);
      } catch (std::exception& e) {
        LOG(DEBUG) << "Reply:\n " << reply.body;
      }

      std::any arg = reply.body;

      // Handle other packets type.
      switch (reply.type) {
        case games::PacketType::GameSpecificData:
        {
          m_receivedReplies.push_back(reply);
          break;
        }

        case games::PacketType::ID:
        {
          // Callback for ID packets
          if (m_callbacks) {
            m_callbacks->IDCallback(arg);
          }
          break;
        }

        case games::PacketType::StartTransaction:
        {
          if (m_transactionState == TransactionState::NotStarted) {
            LOG(DEBUG) << "[DEBUG] Started transaction for " << reply.body << "!\n";
            m_currentAssetKey = reply.body;
            m_transactionState = TransactionState::InProgress;
            m_receivedRawPackets.clear();
          }
          break;
        }

        case games::PacketType::EndTransaction:
        {
          if (m_transactionState == TransactionState::InProgress) {
            LOG(DEBUG) << "[DEBUG] Ended transaction for " << reply.body << "!\n";

            if (!assets::AssetsReceiver::parseAndAddAsset(m_receivedRawPackets, m_currentAssetKey)) {
              std::cerr << "Error adding " << m_currentAssetKey << "!\n";
            }

            // Reset transaction parameters
            m_transactionState = TransactionState::NotStarted;
            m_transactionCounter = 0;

            // Check if more data is available - if yes, receive it as a new transaction
            _requestAsset();
          }
          break;
        }

        case games::PacketType::ListAvailableGames:
          if (m_callbacks) {
            m_callbacks->listAllAvailableGamesCallback(arg);
          }
          break;

        case games::PacketType::CreateLobby:
          if (m_callbacks) {
            m_callbacks->createLobbyCallback(arg);
          }
          break;

        case games::PacketType::GetLobbyDetails:
          if (m_callbacks) {
            m_callbacks->getLobbyDetailsCallback(arg);
          }
          break;

        case games::PacketType::ListOpenLobbies:
          if (m_callbacks) {
            m_callbacks->listOpenLobbiesCallback(arg);
          }
          break;

        case games::PacketType::JoinLobby:
          if (m_callbacks) {
            m_callbacks->joinLobbyCallback(arg);
          }
          break;

        case games::PacketType::ClientDisconnected:
          if (m_callbacks) {
            m_callbacks->clientDisconnectedCallback(arg);
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


bool ClientPacketHandler::_requestAsset() {
  sf::Packet requestPacket;
  games::Request request {
          .type = games::PacketType::DownloadAssets
  };
  requestPacket << request;

  // Send packet to server without requesting a mutex
  sf::Socket::Status retStatus = m_serverSocket.send(requestPacket);
  while (retStatus == sf::Socket::Partial) {
    retStatus = m_serverSocket.send(requestPacket);
  }

  return (retStatus == sf::Socket::Done);
}

void ClientPacketHandler::connectCallbacks(games::ICallbacks* callbacks)
{
  if (callbacks) {
    m_callbacks = callbacks;
  } else {
    err_handler::ErrorLogger::printError("Given callbacks pointer is a nullptr!");
  }
}

} // namespaces
