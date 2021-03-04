#pragma once

#include "Games/ConsoleView.h"

namespace pla::common::games::dice_roller {

class DiceRollerConsoleView : public pla::common::games::ConsoleView
{
public:
  void init() final;
  void viewObject(pla::common::games::ViewObject& object) final;
  void notifyController(std::function<void(std::any)> callback) final;

  [[noreturn]] void runLoop(Controller* controller) final;
};

} // namespaces