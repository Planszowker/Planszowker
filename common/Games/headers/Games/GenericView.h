#pragma once

/* Generic */
#include "Controller.h"

/* STD */
#include <functional>
#include <any>
#include <atomic>

namespace pla::common::games {

class Controller;

/*!
 * @brief View interface class.
 */
class GenericView
{
public:
  /*!
   * @brief Initialization method. Has to be invoked at first place.
   */
  virtual void init() = 0;

  /*!
   * @brief Run view in loop (get user input, handle events, send callbacks).
   */
  virtual void run() = 0;

  /*!
   * @brief Connect controller to view.
   * It has to be invoked before running the view.
   *
   * @param controller Controller pointer
   */
  virtual void connectController(Controller* controller)
  {
    m_controller = controller;
  }

  /*!
   * @brief Update view with controller's object.
   *
   * @param objectFromController Object sent from controller
   */
  virtual void update(const std::any& objectFromController) = 0;

protected:

  Controller* m_controller = nullptr;
};

} // namespaces
