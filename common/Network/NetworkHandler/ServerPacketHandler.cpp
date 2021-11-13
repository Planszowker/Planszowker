#include "ServerPacketHandler.h"

#include "Logger/Logger.h"

using namespace pla::common;
using namespace pla::common::logger;
using namespace pla::common::err_handler;
using namespace pla::common::client_info;

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

  it->second->setBlocking(false);

  Logger::printInfo("Adding new client with IP: " + info.getIpAddress().toString() + ":" + std::to_string(info.getPort()) + " with uniqueID: " + std::to_string(m_lastClientId)
                    + " (" + std::to_string(m_clients.size()) + " / " + std::to_string(m_maxPlayers) + ")");
  ++m_lastClientId;

  return true;
}


void ServerPacketHandler::_heartbeatTask(std::mutex& tcpMutex) {
  while(m_run) {
    std::this_thread::sleep_for(std::chrono::milliseconds (1000));

    const std::lock_guard<std::mutex> lock(tcpMutex);

    // TODO: Maybe refactor for normal for-loop to delete multiple clients in one run
    for (auto& client: m_clients) {
      // TODO: Refactor
      uint8_t type{1}; // HEARTBEAT
      sf::Packet packet;
      packet << type;

      //Logger::printInfo("Sending data to " + std::to_string(client.first));
      sf::Socket::Status clientStatus = client.second->send(packet);

      if (clientStatus != sf::Socket::Done) {
        Logger::printInfo("Deleting client with ID: " + std::to_string(client.first) + " for failed querying ("
                          + std::to_string(m_clients.size() - 1) + " / " + std::to_string(m_maxPlayers) + ")");
        m_clients.erase(client.first);
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
    std::this_thread::sleep_for(std::chrono::milliseconds (20));

    const std::scoped_lock tcpSocketsLock(tcpSocketsMutex);
    for (auto& client : m_clients)
    {
      sf::Packet clientPacket;
      sf::Socket::Status status = client.second->receive(clientPacket);
      while (status == sf::Socket::Partial) {
        status = client.second->receive(clientPacket);
      }

      if (status == sf::Socket::Done) {
        Logger::printDebug("Received packet from " + client.second->getRemoteAddress().toString() + ":"
                           + std::to_string(client.second->getRemotePort()) + " with size of " + std::to_string(clientPacket.getDataSize()));
      }
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

    // Check if we have enough players
    if (m_clients.size() >= m_maxPlayers) {
      m_hasEnoughClientsConnected = true;
    } else {
      m_hasEnoughClientsConnected = false;
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

    // Probably not needed since this task is always blocked on listening...
    std::this_thread::sleep_for(std::chrono::milliseconds (10));
  }
}


} // namespaces
