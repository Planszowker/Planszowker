#pragma once

#include <SFML/Network.hpp>
#include <unordered_map>
#include <future>
#include <mutex>
#include <atomic>

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
    * @brief Constructor with maximum number of players connected at the same time
    */
  explicit NetworkHandler(size_t maxPlayers = 0);

  /*!
   * @brief Get current port
   *
   * @return unsigned short Current port
   */
  [[nodiscard]] unsigned short getPort();

  /*!
   * @brief Method to run handling network in loop (included).
   */
  void run();

  /*!
   *
   */
  void stop();

private:

  /*!
   * @brief Adds a client to a specific container if he doesn't already exists.
   *
   * @param socket Tcp Socket with new client.
   * @return bool True if we added new client. False otherwise.
   */
  bool _addClient(std::shared_ptr<sf::TcpSocket>& socket);

  /*!
   *
   */
  void _handleClientsThread(std::promise<void> statePromise, std::unordered_map<size_t, common::client_info::ClientInfo>& map);

  /*!
   * @brief Initialize function
   */
  void init();

  size_t m_maxPlayers;

  sf::TcpListener m_listener; ///< TCP listener for new connections
  unsigned short m_port; ///< Current used port

  std::unordered_map<size_t, common::client_info::ClientInfo> m_clients; ///< Container to hold information about clients
  size_t m_lastClientId;

  std::mutex m_clientsMutex; ///< Mutex for synchronisation with container's access
  std::atomic<bool> m_runNetworkHandler;
};

} // namespaces
