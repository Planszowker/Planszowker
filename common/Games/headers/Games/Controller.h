#pragma once

/* SFML */
#include <SFML/Network.hpp>

/* STD */
#include <any>
#include <atomic>
#include <mutex>
#include <thread>

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

  virtual ~Controller()
  {
    m_controllerThread.join();
  }

  /*!
   * @brief Initialize method for the Controller.
   * It can be overridden if it is necessary.
   */
  virtual void init() { }

  /*!
   * @brief Runs controller in background.
   * Run method is invoked in another thread.
   * Uses MVC design patter.
   *
   * @see GenericView
   */
  virtual void runInBackground() = 0;

  /*!
   * @brief A callback function used in view.
   *
   * @see GenericView
   */
  virtual void viewCallback(std::any& object) = 0;

  /*!
   * @brief Connect view to controller.
   * It has to be invoked before running the controller.
   *
   * @param view Generic view pointer
   */
  virtual void connectView(GenericView* view)
  {
    m_view = view;
  }

protected:
  std::atomic_bool& m_runThreads; ///< Flag used to sync threads.

  std::thread m_controllerThread; ///< Thread for running controller.

  GenericView* m_view;
};

} // namespaces
