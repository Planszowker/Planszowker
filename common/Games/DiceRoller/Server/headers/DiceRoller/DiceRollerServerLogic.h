#pragma once

#include "NetworkHandler/NetworkHandler.h"

#include <memory>

namespace pla::common::dice_roller {

class DiceRollerServerLogic
{
public:
  DiceRollerServerLogic();

private:
  std::shared_ptr<network::NetworkHandler> m_networkHandler;

};

}
