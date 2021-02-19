#include "NetworkHandler.h"

#include "NetworkQuery/NetworkQuery.h"
#include "Logger/Logger.h"

#include <memory>

using namespace pla::common;
using namespace pla::common::logger;
using namespace pla::common::err_handler;
using namespace pla::common::client_info;

namespace pla::common::network {

NetworkHandler::NetworkHandler()
  : m_lastClientId(0)
{
  if(m_listener.listen(sf::Socket::AnyPort) != sf::Socket::Done) {
    ErrorLogger::printError("Error binding for TCP Listener in NetworkHandler!");
  }

  Logger::printInfo("Successfully created TCP Listener on port: " + std::to_string(m_listener.getLocalPort()));
}


void NetworkHandler::run(const unsigned short& port)
{
  std::shared_ptr<sf::TcpSocket> tcpSocket = std::make_shared<sf::TcpSocket>();

  if(m_listener.accept(*tcpSocket) != sf::Socket::Done) {
    ErrorLogger::printWarning("Error accepting new TCP Socket!");
    return;
  }

  _addClient(*tcpSocket);
}


bool NetworkHandler::_addClient(sf::TcpSocket& socket)
{
  // Create ClientInfo to store information about a client
  ClientInfo info(socket.getRemoteAddress(), socket.getRemotePort());

  // Iterate over all clients and check if we already have one
  for(auto& client : m_clients)
  {
    // If we found one we return false
    if(client.second.getIpAddress() == info.getIpAddress() && client.second.getPort() == info.getPort()) {
      return false;
    }
  }

  // Set client's state
  info.setClientState(ClientStates::Valid);

  // If client doesn't exist, add him to container.
  m_clients.insert(std::make_pair(m_lastClientId, info));
  ++m_lastClientId;

   return true;
}


std::unordered_map<size_t, common::client_info::ClientInfo>& NetworkHandler::getClientsDatabase()
{
  return m_clients;
}


bool NetworkHandler::_queryClient(sf::TcpSocket &socket) {
  return NetworkQuery::queryClient(socket);
}

} // namespaces
