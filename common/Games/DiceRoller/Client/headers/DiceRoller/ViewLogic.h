#pragma once

#include "Games/ViewLogic.h"

namespace pla::common::games::dice_roller {

class DiceRollerViewLogic : public ViewLogic
{
public:
  void update(std::any& object) final;
  void updateView() final;
};

} // namespaces
