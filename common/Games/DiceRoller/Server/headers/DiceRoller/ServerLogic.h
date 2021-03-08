#pragma once

#include "Games/ServerLogic.h"

namespace pla::common::games::dice_roller {

class DiceRollerServerLogic : public ServerLogic
{
public:
  DiceRollerServerLogic() = default;

  void addPlayerId(std::size_t& playerId) final;
  bool checkForAvailableMove(std::size_t& playerId) final;
};

} // namespaces
