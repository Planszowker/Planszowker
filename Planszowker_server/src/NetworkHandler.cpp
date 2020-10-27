#include "NetworkHandler.h"

namespace Planszowker::server::network
{


NetworkHandler::NetworkHandler(const unsigned short& players)
  : m_playersRequired(players)
{

}


void NetworkHandler::run(const unsigned short& port)
{
  _bindSocket(port);
}


void NetworkHandler::_bindSocket(const unsigned short& port)
{
  // If we cannot bind to a specific port we throw exception
  if(m_socket.bind(port) != sf::Socket::Done)
    m_errorLogger.printError("Socket was NOT created successfully! Aborting...");
}


bool NetworkHandler::_addClient(sf::IpAddress& ipAddress, unsigned short& port)
{**
  // Iterate over all clients and check if we already have one
  for(std::vector<server::ClientInfo>::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
  {
    // If we found one we return false
    if(it->getIpAddress() == ipAddress && it->getPort() == port)
      return false;
  }

  // If client doesn't exist, add him to container.
  m_clients.push_back(ClientInfo(ipAddress, port));
  return true;
}


} // namespace