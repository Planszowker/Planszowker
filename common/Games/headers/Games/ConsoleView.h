#pragma once

#include "ViewObject.h"

#include <functional>

namespace pla::common::games {

/*!
 * @brief Console view interface.
 */
class ConsoleView {
public:
  virtual void init() = 0;
  virtual void viewObject(ViewObject& object) = 0;
  virtual void notify(std::function<>())
};

} // namespaces