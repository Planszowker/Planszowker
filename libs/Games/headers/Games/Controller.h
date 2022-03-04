#pragma once

/* SFML */
#include <SFML/Network.hpp>

/* STD */
#include <any>
#include <atomic>
#include <mutex>

namespace pla::games {

class GenericView;

/*!
 * @brief Controller interface class.
 */
class Controller
{
public:
  explicit Controller() : m_run(true)
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

protected:
  std::atomic_bool m_run; ///< Flag used to sync threads.

  std::mutex m_mutex; ///< Mutex for shared resources.
};

} // namespaces
