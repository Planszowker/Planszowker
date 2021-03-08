#pragma once

#include "Games/ConsoleView.h"

#include <any>
#include <mutex>
#include <atomic>

namespace pla::common::games::dice_roller {

class DiceRollerConsoleView : public ConsoleView
{
public:
  void init() final;
  void update(const std::any& object) final;
  void notifyController(std::function<void(std::any)> callback) final;

  void runLoop(Controller* controller, std::atomic_bool& runLoop) final;
};

} // namespaces