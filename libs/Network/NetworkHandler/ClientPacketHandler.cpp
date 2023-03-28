#include "ClientPacketHandler.h"

#include <Games/CommObjects.h>
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

using namespace games::json_entries;

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

  std::thread backgroundThread{&ClientPacketHandler::_backgroundTask, this};
  m_backgroundThread = std::move(backgroundThread);
}


void ClientPacketHandler::stop() {
  m_run = false;
}


void ClientPacketHandler::_backgroundTask()
{
  // This method will receive and send packets to server
  while(m_run) {
    std::this_thread::sleep_for(std::chrono::milliseconds (10));

    TimeLogger logger(GET_CURRENT_FUNCTION_NAME());
    std::scoped_lock tcpSocketsLock{m_tcpSocketsMutex};

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
        LOG(ERROR) << "[PacketHandler] Corrupted packet!\n";
        continue;
      }

      // If received status is not Success, we shouldn't handle it.
      if (reply.type == games::PacketType::Heartbeat) {
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
            try {
              nlohmann::json replyJson = nlohmann::json::parse(reply.body);
              LOG(DEBUG) << "[DEBUG] Started transaction for " << replyJson[ASSET_NAME] << "!\n";
              m_currentAssetKey = replyJson[ASSET_NAME];
              m_transactionState = TransactionState::InProgress;
              m_receivedRawPackets.clear();
            } catch (std::exception& e) { }
          }
          break;
        }

        case games::PacketType::EndTransaction:
        {
          if (m_transactionState == TransactionState::InProgress) {
            try {
              nlohmann::json replyJson = nlohmann::json::parse(reply.body);
              auto assetName = replyJson[ASSET_NAME].get<std::string>();
              auto assetType = replyJson[ASSET_TYPE].get<std::string>();

              LOG(DEBUG) << "[DEBUG] Ended transaction for " << assetName << " (" << assetType << ")";

              if (assetType == "Image") {
                if (!assets::AssetsReceiver::parseAndAddAsset(m_receivedRawPackets, m_currentAssetKey)) {
                  LOG(ERROR) << "Error adding " << m_currentAssetKey << "!";
                }
              } else if (assetType == "BoardDescription"){
                if (!assets::AssetsReceiver::addBoardDescription(m_receivedRawPackets, m_currentAssetKey)) {
                  LOG(ERROR) << "Error adding " << m_currentAssetKey << "!";
                }
              }

              // Reset transaction parameters
              m_transactionState = TransactionState::NotStarted;
              m_transactionCounter = 0;

              if (m_callbacks) {
                LOG(DEBUG) << "Callback for EndTransaction";
                m_callbacks->endTransactionCallback(arg);
              }

              // Check if more data is available - if yes, receive it as a new transaction
              _requestAsset();
            } catch (std::exception& e) { }
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

        case games::PacketType::DisconnectClient:
          if (m_callbacks) {
            m_callbacks->disconnectClientCallback(arg);
          }
          break;

        case games::PacketType::StartGame:
          if (m_callbacks) {
            m_callbacks->startGameCallback(arg);
          }
          break;

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
  }
}

} // namespaces
