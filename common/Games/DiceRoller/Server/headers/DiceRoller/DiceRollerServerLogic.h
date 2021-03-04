#pragma once

#include "Games/ServerLogic.h"
#include "NetworkHandler/NetworkHandler.h"

#include <memory>

namespace pla::common::games::dice_roller {

class DiceRollerServerLogic : public ServerLogic
{
public:
  DiceRollerServerLogic();

  void run() final;
  void stop() final { };

private:
  std::shared_ptr<network::NetworkHandler> m_networkHandler;

  static constexpr size_t MaxPlayers = 2;
};

} // namespaces
