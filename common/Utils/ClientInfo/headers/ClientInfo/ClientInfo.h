#pragma once

#include <SFML/Network.hpp>

#include <memory>

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
  [[nodiscard]] const sf::IpAddress& getIpAddress();

  /*!
   *  \brief Getter of client's port.
   *
   *  \return Reference for client's port.
   */
  [[nodiscard]] const unsigned short& getPort() const;


  /*!
   *  \brief Getter of client's state
   *
   *  \return Current client's state
   */
  [[nodiscard]] ClientStates getClientState() const;

  /*!
   *  \brief Setter of client's state
   *
   *  \param state New client's state
   */
  void setClientState(const ClientStates& state);

  /*!
   *  \brief Getter of client's socket.
   *
   *  \return Reference for shared pointer for client's socket.
   */
  std::shared_ptr<sf::TcpSocket>& getClientSocket();

  /*!
   *  \brief Setter of client's socket.
   *
   *  \param socket Reference for shared pointer for client's socket.
   */
  void setClientSocket(std::shared_ptr<sf::TcpSocket>& socket);

private:
  sf::IpAddress m_Ip; ///< Used to store client's IP address.
  unsigned short m_Port; ///< Used to store client's port.

  std::shared_ptr<sf::TcpSocket> m_socket; ///< Client's socket.

  ClientStates m_state;
};

} // namespaces
