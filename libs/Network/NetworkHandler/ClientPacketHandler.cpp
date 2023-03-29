#include "ClientPacketHandler.h"

#include <Games/CommObjects.h>
#include <TimeMeasurement/TimeLogger.h>
#include <CompilerUtils/FunctionInfoExtractor.h>
#include <AssetsManager/AssetsReceiver.h>
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
        // If it's a corrupted packet, just continue.
        LOG(ERROR) << "[PacketHandler] Corrupted packet!\n";
        continue;
      }

      // If received status is not Success, we shouldn't handle it.
      if (reply.type == games::PacketType::Heartbeat) {
        continue;
      }

      if (reply.type != games::PacketType::DownloadAssets) {
        try {
          LOG(DEBUG) << "Reply:\n" << nlohmann::json::parse(reply.body).dump(4);
        } catch (std::exception &e) {
          LOG(DEBUG) << "Reply:\n " << reply.body;
        }
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

        case games::PacketType::DownloadAssets:
        {
          try {
            nlohmann::json replyJson = nlohmann::json::parse(reply.body);
            assets::AssetsReceiver::parseAndAddAssets(replyJson);
            if (m_callbacks) {
              m_callbacks->downloadAssetsCallback(std::any{});
            }
          } catch (std::exception& e) { }
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
