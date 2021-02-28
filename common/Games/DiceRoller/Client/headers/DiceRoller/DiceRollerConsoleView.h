#pragma once

#include "Games/ConsoleView.h"

namespace pla::common::games::dice_roller {

class DiceRollerConsoleView : public pla::common::games::ConsoleView
{
public:
  void viewObject(pla::common::games::ViewObject& object) final;
};

} // namespaces