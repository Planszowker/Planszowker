#include "NetworkHandler.h"

#include "NetworkQuery/NetworkQuery.h"
#include "Logger/Logger.h"

#include <memory>
#include <thread>
#include <future>

using namespace pla::common;
using namespace pla::common::logger;
using namespace pla::common::err_handler;
using namespace pla::common::client_info;

namespace pla::common::network {

NetworkHandler::NetworkHandler(size_t maxPlayers)
  : m_lastClientId(0)
  , m_maxPlayers(maxPlayers)
  , m_runNetworkHandler(true)
  , m_delegateLoop(false)
  , m_port(0)
{
  init();
}


void NetworkHandler::run()
{
  std::thread handleClientThread(&NetworkHandler::_handleClientsThread, this, std::ref(m_clients), std::ref(m_clientsMutex));
  handleClientThread.detach();

  // Loop to handle client connections
  while(m_runNetworkHandler)
  {
    std::shared_ptr<sf::TcpSocket> tcpSocket = std::make_shared<sf::TcpSocket>();

    // Blocks until new connection
    if (m_listener.accept(*tcpSocket) != sf::Socket::Done) {
      ErrorLogger::printWarning("Error accepting new TCP Socket!");
      return;
    }

    if (!m_maxPlayers || m_clients.size() < m_maxPlayers) {
      if(!_addClient(tcpSocket)) {
        ErrorLogger::printWarning("Error adding new client from " + tcpSocket->getRemoteAddress().toString() + ":" + std::to_string(tcpSocket->getRemotePort()));
      }
    } else {
      Logger::printInfo("Maximum number of players reached!");
    }

    // If we reach enough amount of players, break out of loop and process delegating packets
    if (m_clients.size() == m_maxPlayers) {
      break;
    }
  }

  // Loop to handle packets received from clients
  m_delegateLoop = true;
  while(m_runNetworkHandler)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds (200));
    const std::lock_guard<std::mutex> lock(m_clientsMutex);

    for (auto& client : m_clients)
    {
      sf::Packet clientPacket;
      sf::Socket::Status status = client.second.getClientSocket()->receive(clientPacket);
      while (status == sf::Socket::Partial) {
        status = client.second.getClientSocket()->receive(clientPacket);
      }

      if (status == sf::Socket::Done) {
        Logger::printDebug("Received packet from " + client.second.getIpAddress().toString() + ":" + std::to_string(client.second.getPort()));
      }
    }
  }
}


bool NetworkHandler::_addClient(std::shared_ptr<sf::TcpSocket>& socket) {
  const std::lock_guard<std::mutex> lock(m_clientsMutex);

  // Create ClientInfo to store information about a client
  ClientInfo info(socket->getRemoteAddress(), socket->getRemotePort());

  // Iterate over all clients and check if we already have one
  for (auto& client : m_clients) {
    // If we found one we return false
    if (client.second.getIpAddress() == info.getIpAddress() && client.second.getPort() == info.getPort()) {
      return false;
    }
  }

  socket->setBlocking(false);

  // Set socket's reference
  info.setClientSocket(socket);

  // If client doesn't exist, add him to container.
  m_clients.insert(std::make_pair(m_lastClientId, info));
  Logger::printInfo("Adding new client with IP: " + info.getIpAddress().toString() + ":" + std::to_string(info.getPort()) + " with uniqueID: " + std::to_string(m_lastClientId)
                    + " (" + std::to_string(m_clients.size()) + " / " + std::to_string(m_maxPlayers) + ")");
  ++m_lastClientId;

  return true;
}


void NetworkHandler::_handleClientsThread(std::unordered_map<size_t, common::client_info::ClientInfo>& map, std::mutex& clientMutex) {
  while(m_runNetworkHandler) {
    std::this_thread::sleep_for(std::chrono::milliseconds (200));

    const std::lock_guard<std::mutex> lock(m_clientsMutex);

    for (auto it = map.begin(); it != map.end(); ++it) {
      bool result = NetworkQuery::queryClient(it->second.getClientSocket());

      if (!result) {
        Logger::printInfo("Deleting client with ID: " + std::to_string(it->first) + " for failed querying ("
                          + std::to_string(m_clients.size() - 1) + " / " + std::to_string(m_maxPlayers) + ")");
        map.erase(it);

        if (m_delegateLoop) {
          Logger::printInfo("Exiting NetworkHandler!");
          m_runNetworkHandler = false;
        }

        break;
      }
    }
  }
}


void NetworkHandler::stop() {
  m_runNetworkHandler = false;
}


void NetworkHandler::init()
{
  if(m_listener.listen(sf::Socket::AnyPort) != sf::Socket::Done) {
    ErrorLogger::printError("Error binding for TCP Listener in NetworkHandler!");
  }

  m_port = m_listener.getLocalPort();

  Logger::printInfo("Successfully created TCP Listener on port: " + std::to_string(m_port));
}


unsigned short NetworkHandler::getPort() const
{
  return m_port;
}

} // namespaces
