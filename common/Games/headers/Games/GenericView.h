#pragma once

/* Generic */
#include "Controller.h"

/* STD */
#include <functional>
#include <any>
#include <atomic>

namespace pla::common::games {

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
};

} // namespaces
