#pragma once

#include <SFML/Network.hpp>

#include <memory>

namespace pla::client_info {

class ClientInfo
{
public:
  /*!
   * @brief Constructor of Client class.
   *
   * This class will store IP and PORT for communication client <=> server.
   * @param ipAddress Client's IP address.
   * @param port Client's port.
   * @param id Client's ID.
   */
  ClientInfo(const sf::IpAddress& ipAddress, const unsigned short& port, size_t id);

  /*!
   * @brief Getter of client's IP address.
   *
   * @return Const reference for client's IP address.
   */
  [[nodiscard]] inline const sf::IpAddress& getIpAddress()
  {
    return m_ip;
  }

  /*!
   * @brief Getter of client's port.
   *
   * @return Reference for client's port.
   */
  [[nodiscard]] inline const unsigned short& getPort() const
  {
    return m_port;
  }


  /*!
   * @brief Getter of client's id.
   *
   * @return Client's ID.
   */
   [[nodiscard]] inline size_t getId() const
  {
     return m_id;
  }


private:
  sf::IpAddress m_ip; ///< Used to store client's IP address.
  unsigned short m_port; ///< Used to store client's port.
  size_t m_id; ///< Unique player ID.
};

} // namespaces
