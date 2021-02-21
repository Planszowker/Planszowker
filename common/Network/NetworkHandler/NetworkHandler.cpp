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

NetworkHandler::NetworkHandler()
  : m_lastClientId(0)
  , m_runNetworkHandler(true)
{
  if(m_listener.listen(sf::Socket::AnyPort) != sf::Socket::Done) {
    ErrorLogger::printError("Error binding for TCP Listener in NetworkHandler!");
  }

  Logger::printInfo("Successfully created TCP Listener on port: " + std::to_string(m_listener.getLocalPort()));
}


void NetworkHandler::run()
{
  std::promise<void> stateIndicator;
  std::future<void> stateIndicatorFuture = stateIndicator.get_future();

  std::thread handleClientThread(&NetworkHandler::_handleClientsThread, this, std::move(stateIndicator));

  while(m_runNetworkHandler) {
    std::shared_ptr<sf::TcpSocket> tcpSocket = std::make_shared<sf::TcpSocket>();

    // Blocks until new connection
    if (m_listener.accept(*tcpSocket) != sf::Socket::Done) {
      ErrorLogger::printWarning("Error accepting new TCP Socket!");
      return;
    }

    _addClient(*tcpSocket);
  }

  stateIndicatorFuture.wait();
  handleClientThread.join();
}


bool NetworkHandler::_addClient(sf::TcpSocket& socket)
{
  std::lock_guard<std::mutex> lock(m_clientsMutex);

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

  // If client doesn't exist, add him to container.
  m_clients.insert(std::make_pair(m_lastClientId, info));
  Logger::printInfo("Adding new client with IP: " + info.getIpAddress().toString() + ":" + std::to_string(info.getPort()) + " with uniqueID: " + std::to_string(m_lastClientId));
  ++m_lastClientId;

   return true;
}


bool NetworkHandler::_removeClient(size_t clientId)
{
  size_t ret = m_clients.erase(clientId);

  return ret != 0;
}


void NetworkHandler::_handleClientsThread(std::promise<void> statePromise) {
  while(m_runNetworkHandler) {
    const std::lock_guard<std::mutex> lock(m_clientsMutex);

    for (auto &client : m_clients) {
      bool result = NetworkQuery::queryClient(*client.second.getClientSocket());
      if (!result) {
        Logger::printInfo("Deleting client with ID: " + std::to_string(client.first) + " for failed querying");
        _removeClient(client.first);
        continue;
      }
    }
  }

  statePromise.set_value();
}


void NetworkHandler::stop() {
  m_runNetworkHandler = false;
}

} // namespaces
