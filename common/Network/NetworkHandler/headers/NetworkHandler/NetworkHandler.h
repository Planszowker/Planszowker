#pragma once

#include <SFML/Network.hpp>
#include <unordered_map>

#include "ClientInfo/ClientInfo.h"
#include "ErrorHandler/ErrorLogger.h"

namespace pla::common::network {

/*!
 *  @brief Class for handling network. Should be used in another thread.
 *
 *  This class throw exceptions, so they have to be caught by try-catch block.
 *  This class uses SFML Networking module for communication.
 */
class NetworkHandler
{
public:

  /*!
   * @brief Constructor for NetworkHandler.
   */
   NetworkHandler();

  /*!
   * @brief Method to run handling network in loop.
   *
   * The thread will be blocked till it gets new data
   */
  void run(const unsigned short& port);

  /*!
   * @brief Method to get client database
   *
   * @return unordered_map Container with all clients
   */
  std::unordered_map<size_t, common::client_info::ClientInfo>& getClientsDatabase();

private:

  /*!
   * @brief Adds a client to a specific container if he doesn't already exists.
   *
   * @param socket Tcp Socket with new client.
   * @return bool True if we added new client. False otherwise.
   */
  bool _addClient(sf::TcpSocket& socket);

  /*!
   * @brief Remove client from database.
   *
   * @param clientId Unique ID of client
   * @return True on successful deletion, false otherwise.
   */
  bool _removeClient(size_t clientId);

  /*!
   * @brief Query a client to see if it's still alive.
   *
   * @param socket Client's socket.
   * @return True if client is still alive, false otherwise.
   */
  bool _queryClient(sf::TcpSocket& socket);

  sf::TcpListener m_listener; ///< TCP listener for new connections
  sf::Packet m_packet; ///< Packet received
  std::unordered_map<size_t, common::client_info::ClientInfo> m_clients; ///< Container to hold information about clients
  size_t m_lastClientId;
};

} // namespaces
