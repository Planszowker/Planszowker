#pragma once

#include "Games/Controller.h"
#include "DiceRollerConsoleView.h"

#include <memory>
#include <any>

namespace pla::common::games::dice_roller {

class DiceRollerController : public Controller
{
public:
  void run() final;
  void attachView(ConsoleView* view) final;
  void viewCallback(std::any object) final;

private:
  void updateView() final;

  void updateModel() final;
  void getUpdatedModel() final;

private:
  DiceRollerConsoleView *m_view;
};

} // namespace
