#pragma once

#include <SFML/Network.hpp>
#include <unordered_map>

#include "ClientInfo/ClientInfo.h"
#include "ErrorHandler/ErrorLogger.h"

namespace pla::server {

/*!
 *  \brief Class for handling network. Should be used in another thread.
 *
 *  This class throw exceptions, so they have to be caught by try-catch block.
 *  This class uses SFML Networking module for communication.
 */
class NetworkHandler
{
public:

  /*!
   *  \brief Constructor for NetworkHandler.
   */
   NetworkHandler();

  /*!
   *  \brief Method to run handling network in loop.
   *
   *  The thread will be blocked till it gets new data
   */
  void run(const unsigned short& port);
private:

  /*!
   *  \brief Method to bind a socket to a specific port.
   *
   *  If we cannot bind to a specific port we throw an exception.
   *
   *  \param port Port to bind a socket
   *
   *  \see ErrorHandler, ExceptionThrower
   */
  void _bindSocket(const unsigned short& port);

  /*!
   *  \brief Method to receive data via a socket.
   *
   *  \param[out] packet Reference for packet received.
   *  \param[out] ipAddress IP Address of incoming connection.
   *  \param[out] port Port of incoming connection.
   */
  void _receiveData(sf::Packet& packet, sf::IpAddress& ipAddress, unsigned short& port);


  /*!
   *  \brief Adds a client to a specific container if he doesn't already exists.
   *
   *  \param ipAddress IP Client's IP address.
   *  \param port Client's port.
   *  \return bool True if we added new client. False otherwise.
   */
  bool _addClient(sf::IpAddress& ipAddress, unsigned short& port);


  sf::TcpListener m_listener; ///< TCP listener for new connections
  sf::Packet m_packet; ///< Packet received
  std::unordered_map<size_t, common::client_info::ClientInfo> m_clients; ///< Container to hold information about clients

  common::err_handler::ErrorLogger m_errorLogger; ///< ErrorHandler for exceptions throwing

  unsigned short m_playersRequired;
};

} // namespaces
