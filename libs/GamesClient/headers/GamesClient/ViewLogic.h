#pragma once

/* Generic */
#include "Games/ViewLogic.h"

/* STD */
#include <any>

namespace pla::common::games::dice_roller {

class DiceRollerViewLogic : public pla::games::ViewLogic
{
public:
  void updateModel(const std::any& modelUpdateData) final;

  std::any getUpdatedModel() final;
};

} // namespaces