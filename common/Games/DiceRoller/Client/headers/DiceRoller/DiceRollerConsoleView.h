#pragma once

#include "Games/ConsoleView.h"

#include <any>
#include <mutex>

namespace pla::common::games::dice_roller {

class DiceRollerConsoleView : public ConsoleView
{
public:
  void init() final;
  void update(const std::any& object) final;
  void notifyController(std::function<void(std::any)> callback) final;

  [[noreturn]] void runLoop(Controller* controller) final; // TODO: Remove noreturn
};

} // namespaces