#ifndef PLANSZOWKER_SERVER_NETWORKHANDLER_H
#define PLANSZOWKER_SERVER_NETWORKHANDLER_H

#include <SFML/Network.hpp>
#include <vector>

#include "ClientInfo.h"
#include "ErrorLogger.h"


namespace Planszowker::server::network
{

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
   *
   *  \param players How many players are required for specific game
   */
   NetworkHandler(const unsigned short& players);

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
   *  \see ErrorLogger, ExceptionThrower
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


  sf::UdpSocket m_socket;                    ///< Socket used to connect to clients
  sf::Packet m_packet;                       ///< Packet received
  std::vector<server::ClientInfo> m_clients; ///< Vector to hold information about clients

  common::ErrorLogger m_errorLogger;         ///< ErrorLogger for exceptions throwing

  unsigned short m_playersRequired;
};


} // namespace

#endif //PLANSZOWKER_SERVER_NETWORKHANDLER_H
