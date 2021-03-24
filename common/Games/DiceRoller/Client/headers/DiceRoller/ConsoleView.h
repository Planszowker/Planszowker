#pragma once

/* Generic */
#include "Games/GenericView.h"

/* STD */
#include <any>
#include <mutex>
#include <atomic>

namespace pla::common::games::dice_roller {

class DiceRollerConsoleView : public GenericView
{
public:
  void init() final;
  void update(const std::any& object) final;
  void notifyController(std::function<void(std::any)> callback) final;

  void runLoop(Controller* controller, std::atomic_bool& runLoop) final;
};

} // namespaces
