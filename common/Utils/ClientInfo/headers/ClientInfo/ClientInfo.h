#pragma once

#include <SFML/Network.hpp>

namespace pla::common::client_info {

class ClientInfo
{
public:
  /*!
   *  \brief Constructor of Client class.
   *
   *  This class will store IP and PORT for communication client <=> server.
   *  \param ipAddr Client's IP address.
   *  \param port Client's port.
   */
  ClientInfo(const sf::IpAddress& ipAddr, const unsigned short& port);

  /*!
   *  \brief Getter of client's IP address.
   *
   *  \return Reference for client's IP address.
   */
  const sf::IpAddress& getIpAddress();

  /*!
   *  \brief Getter of client's port.
   *
   *  \return Reference for client's port.
   */
  const unsigned short& getPort();

private:
  sf::IpAddress m_Ip;     ///< Used to store client's IP address.
  unsigned short m_Port;  ///< Used to store client's port.
};

} // namespaces
