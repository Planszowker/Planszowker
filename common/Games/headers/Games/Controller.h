#pragma once

/* SFML */
#include <SFML/Network.hpp>

/* STD */
#include <any>
#include <atomic>
#include <mutex>

#include "GenericView.h"

namespace pla::common::games {

class GenericView;

/*!
 * @brief Controller interface class.
 */
class Controller
{
public:
  explicit Controller(std::atomic_bool& runThreads)
    : m_runThreads(runThreads)
    , m_view(nullptr)
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
   * @brief Connect view to controller.
   *
   * @param view Generic view pointer
   */
  virtual void connectView(GenericView* view)
  {
    m_view = view;
  }

protected:
  std::atomic_bool& m_runThreads; ///< Flag used to sync threads.

  GenericView* m_view;

  std::mutex m_mutex; ///< Mutex for shared resources.
};

} // namespaces
