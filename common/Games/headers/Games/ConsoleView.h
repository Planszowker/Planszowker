#pragma once

#include "GenericView.h"
#include "ViewObject.h"
#include "Controller.h"

#include <functional>
#include <any>
#include <atomic>

namespace pla::common::games {

class Controller;

/*!
 * @brief Console view interface.
 */
class ConsoleView : public GenericView
{
};

} // namespaces