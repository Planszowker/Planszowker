#pragma once

#include "NetworkHandler/NetworkHandler.h"

#include <memory>

namespace pla::common::games::dice_roller {

class DiceRollerServerLogic
{
public:
  DiceRollerServerLogic();

private:
  std::shared_ptr<network::NetworkHandler> m_networkHandler;

  static constexpr size_t MaxPlayers = 2;
};

} // namespaces
