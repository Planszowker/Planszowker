#include "DiceRollerServerLogic.h"

namespace pla::common::dice_roller {

DiceRollerServerLogic::DiceRollerServerLogic()
{
  m_networkHandler = std::shared_ptr<network::NetworkHandler>(std::make_shared<network::NetworkHandler>(DiceRollerServerLogic::MaxPlayers));
}

} // namespaces