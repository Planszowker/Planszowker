#pragma once

/* Generic */
#include "Games/ViewLogic.h"

/* STD */
#include <any>

namespace pla::common::games::dice_roller {

class DiceRollerViewLogic : public ViewLogic
{
public:
  void updateModel(const std::any& modelUpdateData) final;

  const std::any& getUpdatedModel() final;
};

} // namespaces
