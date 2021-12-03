#pragma once

/* Generic */
#include "Games/GraphicalView.h"

/* STD */
#include <any>
#include <mutex>
#include <atomic>

namespace pla::common::games::dice_roller {

class DiceRollerConsoleView : public GenericView
{
public:
  DiceRollerConsoleView() = default;
  virtual ~DiceRollerConsoleView() = default;

  void init() final;
  void update(const std::any& object) final;

  void runLoop(Controller* controller, std::atomic_bool& runLoop) final;

private:
  void notifyController(std::function<void(std::any&)> callback) final;

  int m_inputType{0};
};

} // namespaces
