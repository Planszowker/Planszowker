#pragma once

#include "ViewObject.h"
#include "Controller.h"

#include <functional>
#include <any>

namespace pla::common::games {

class Controller;

/*!
 * @brief Console view interface.
 */
class ConsoleView {
public:
  virtual void init() = 0;
  virtual void update(const std::any& object) = 0;
  virtual void notifyController(std::function<void(std::any)> callback) = 0;
  virtual void runLoop(Controller* controller) = 0;
};

} // namespaces