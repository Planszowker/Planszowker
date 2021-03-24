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
   * @brief Initialization method. Should be invoked at first place.
   */
  virtual void init() = 0;

  /*!
   * @brief Notify controller about a recent event.
   *
   * @param callback Controller's function that should be invoked.
   */
  virtual void notifyController(std::function<void(std::any)> callback) = 0;

  /*!
   * @brief Run view in loop (get user input, handle events, send callbacks).
   *
   * @param controller Pointer to a controller.
   * @param runLoop Used for thread syncing.
   */
  virtual void runLoop(Controller* controller, std::atomic_bool& runLoop) = 0;

  /*!
   * @brief Update rendering.
   *
   * @ param object Game-specific object that holds required data.
   */
  virtual void update(const std::any& object) = 0;
};

} // namespaces
