#pragma once

#include "Controller.h"

#include <functional>
#include <any>
#include <atomic>

namespace pla::common::games {

class Controller;

/*!
 * @brief Interface class for views
 */
class GenericView
{
public:
  virtual void init() = 0;

  virtual void notifyController(std::function<void(std::any)> callback) = 0;
  virtual void runLoop(Controller* controller, std::atomic_bool& runLoop) = 0;

  virtual void update(const std::any& object) = 0;
};

} // namespaces
