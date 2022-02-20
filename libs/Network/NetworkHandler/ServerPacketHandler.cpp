#include "ServerPacketHandler.h"

#include "Logger/Logger.h"
#include "Games/Objects.h"
#include "TimeMeasurement/TimeLogger.h"
#include "CompilerUtils/FunctionInfoExtractor.h"

using namespace pla::common;
using namespace pla::common::logger;
using namespace pla::common::err_handler;
using namespace pla::common::client_info;
using namespace pla::common::time_measurement;

namespace pla::common::network {

ServerPacketHandler::ServerPacketHandler(size_t maxPlayers)
  : PacketHandler()
  , m_lastClientId(0)
  , m_maxPlayers(maxPlayers)
  , m_port(0)
  , m_hasEnoughClientsConnected(false)
{
  if(m_listener.listen(sf::Socket::AnyPort) != sf::Socket::Done) {
    ErrorLogger::printError("Error binding for TCP Listener in ServerPacketHandler!");
  }

  m_port = m_listener.getLocalPort();

  Logger::printInfo("Successfully created TCP Listener on port: " + std::to_string(m_port));
}


ServerPacketHandler::~ServerPacketHandler()
{
  m_backgroundThread.join();
  m_newConnectionThread.join();
  m_heartbeatThread.join();
}


void ServerPacketHandler::runInBackground()
{
  m_run = true;

  // Create task for handling game-data exchange
  std::thread backgroundThread{&ServerPacketHandler::_backgroundTask, this, std::ref(m_tcpSocketsMutex)};
  m_backgroundThread = std::move(backgroundThread);

  // Create task for handling heartbeat packets
  std::thread heartbeatThread(&ServerPacketHandler::_heartbeatTask, this, std::ref(m_tcpSocketsMutex));
  m_heartbeatThread = std::move(heartbeatThread);

  // Create task for adding new clients
  std::thread newConnectionThread(&ServerPacketHandler::_newConnectionTask, this, std::ref(m_tcpSocketsMutex));
  m_newConnectionThread = std::move(newConnectionThread);
}


bool ServerPacketHandler::_addClient(std::shared_ptr<sf::TcpSocket>& newSocket) {
  // Create ClientInfo to store information about a client
  ClientInfo info(newSocket->getRemoteAddress(), newSocket->getRemotePort(), m_lastClientId);

  // Iterate over all clients and check if we already have one
  for (auto& client : m_clients) {
    // If we found one we return false
    if (client.second->getRemoteAddress() == newSocket->getRemoteAddress()
        && client.second->getRemotePort() == newSocket->getRemotePort())
    {
      return false;
    }
  }

  // If client doesn't exist, add him to container.
  auto[it, inserted] = m_clients.emplace(std::make_pair(m_lastClientId, newSocket));
  if (!inserted) {
    return false;
  }

  // Add to client IDs container
  m_clientIds.push_back(m_lastClientId);

  it->second->setBlocking(false);

  Logger::printInfo("Adding new client with IP: " + info.getIpAddress().toString() + ":" + std::to_string(info.getPort()) + " with uniqueID: " + std::to_string(m_lastClientId)
                    + " (" + std::to_string(m_clients.size()) + " / " + std::to_string(m_maxPlayers) + ")");
  ++m_lastClientId;

  return true;
}


void ServerPacketHandler::_heartbeatTask(std::mutex& tcpMutex) {
  while(m_run) {
    std::this_thread::sleep_for(std::chrono::milliseconds (1000));

    TimeLogger logger(GET_CURRENT_FUNCTION_NAME());
    const std::lock_guard<std::mutex> lock(tcpMutex);

    // TODO: Maybe refactor for normal for-loop to delete multiple clients in one run
    for (auto& client: m_clients) {
      // HEARTBEAT
      sf::Packet packet;
      games::Reply heartbeatReply {
        .type = games::PacketType::Heartbeat,
        .status = games::ReplyType::Success
      };
      packet << heartbeatReply;

      //Logger::printInfo("Sending data to " + std::to_string(client.first));
      sf::Socket::Status clientStatus = client.second->send(packet);

      if (clientStatus != sf::Socket::Done) {
        Logger::printInfo("Deleting client with ID: " + std::to_string(client.first) + " for failed querying ("
                          + std::to_string(m_clients.size() - 1) + " / " + std::to_string(m_maxPlayers) + ")");

        size_t keyToRemove = client.first;

        m_clients.erase(keyToRemove);

        // Also remove from client IDs container
        std::erase(m_clientIds, keyToRemove);

        break; // Break because erasing invalidates iterator
      }
    }
  }
}


void ServerPacketHandler::stop() {
  m_run = false;
}


void ServerPacketHandler::_backgroundTask(std::mutex &tcpSocketsMutex) {
  while (m_run)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds (10));

    TimeLogger logger(GET_CURRENT_FUNCTION_NAME());
    const std::scoped_lock tcpSocketsLock(tcpSocketsMutex);
    for (auto& client : m_clients)
    {
      sf::Packet clientPacket;
      sf::Socket::Status status = client.second->receive(clientPacket);
      while (status == sf::Socket::Partial) {
        status = client.second->receive(clientPacket);
      }

      if (status != sf::Socket::Done) {
        continue;
      }

      // Add received packets to map
      auto packetsIt = m_packets.find(client.first);

      if (packetsIt == m_packets.end()) { // If we don't have client in current map
        std::deque<sf::Packet> deque;
        auto [it, inserted] = m_packets.insert({client.first, deque});
        if (inserted) {
          packetsIt = it;
        }
      }

      if (packetsIt != m_packets.end()) {
        packetsIt->second.push_back(clientPacket);
      }

      Logger::printDebug("Received packet from " + client.second->getRemoteAddress().toString() + ":"
                         + std::to_string(client.second->getRemotePort()) + " with size of " + std::to_string(clientPacket.getDataSize()));
    }
  }
}

void ServerPacketHandler::_newConnectionTask(std::mutex &tcpSocketsMutex) {
  while(m_run) {
    std::shared_ptr<sf::TcpSocket> newTcpSocket = std::make_shared<sf::TcpSocket>();

    // Blocks until new connection is made
    if (m_listener.accept(*newTcpSocket) != sf::Socket::Done) {
      ErrorLogger::printWarning("Error accepting new TCP Socket!");
      return;
    }

    const std::scoped_lock tcpSocketsLock(tcpSocketsMutex);
    if (!m_hasEnoughClientsConnected) {
      if (!_addClient(newTcpSocket)) {
        ErrorLogger::printWarning("Error adding new client from " + newTcpSocket->getRemoteAddress().toString() + ":" +
                                  std::to_string(newTcpSocket->getRemotePort()));
      }
    } else {
      Logger::printInfo("Maximum number of players reached!");
    }

    // Check if we have enough players
    if (m_clients.size() >= m_maxPlayers) {
      m_hasEnoughClientsConnected = true;
    } else {
      m_hasEnoughClientsConnected = false;
    }

    // Probably not needed since this task is always blocked on listening...
    std::this_thread::sleep_for(std::chrono::milliseconds (10));
  }
}

ServerPacketHandler::packetMap ServerPacketHandler::getPackets(std::vector<size_t> &keys) {
  const std::scoped_lock tcpSocketsLock(m_tcpSocketsMutex);

  // Retrieve keys
  keys = m_clientIds;

  packetMap returnMap = m_packets;

  // Clear packets from this class
  m_packets.clear();

  return returnMap;
}

void ServerPacketHandler::sendPacketToEveryClients(sf::Packet &packet) {
  TimeLogger logger(GET_CURRENT_FUNCTION_NAME());
  const std::scoped_lock tcpSocketsLock(m_tcpSocketsMutex);

  for (const auto& client: m_clients) {
    sf::Socket::Status status = client.second->send(packet);
    while (status == sf::Socket::Partial) {
      status = client.second->send(packet);
    }
  }
}

void ServerPacketHandler::sendPacketToClient(size_t clientId, sf::Packet &packet)
{
  TimeLogger logger(GET_CURRENT_FUNCTION_NAME());
  const std::scoped_lock tcpSocketsLock(m_tcpSocketsMutex);

  auto clientIt = m_clients.find(clientId);
  if (clientIt != m_clients.end()) {
    sf::Socket::Status status = clientIt->second->send(packet);
    while (status == sf::Socket::Partial) {
      status = clientIt->second->send(packet);
    }
  }
}


} // namespaces
