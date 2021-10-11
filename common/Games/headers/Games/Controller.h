#pragma once

/* SFML */
#include <SFML/Network.hpp>

/* STD */
#include <any>
#include <atomic>
#include <mutex>

namespace pla::common::games {

class GenericView;

/*!
 * @brief Controller interface class.
 */
class Controller
{
public:
  explicit Controller(sf::TcpSocket& serverSocket)
    : m_serverSocket(serverSocket)
    , m_runController(true)
  {
  }

  /*!
   * @brief Runs controller.
   * It is responsible for creating a View and a Model (Logic).
   * It runs in an infinite loop and receives data from connected server.
   * Uses MVC design patter.
   *
   * @see GenericView
   * @see ViewLogic
   */
  virtual void run() = 0;

  /*!
   * @brief A callback function used in view.
   *
   * @see GenericView
   */
  virtual void viewCallback(std::any& object) = 0;

  /*!
   * @brief Function that runs in separate thread and received server's data.
   */
  virtual void receiveThread(std::mutex& mutex) = 0;

protected:
  sf::TcpSocket& m_serverSocket; ///< TCP Socket that is used to receive data.

  std::atomic_bool m_runController; ///< Flag used to sync threads.

  std::mutex m_mutex; ///< Mutex for shared resources.
};

} // namespaces
