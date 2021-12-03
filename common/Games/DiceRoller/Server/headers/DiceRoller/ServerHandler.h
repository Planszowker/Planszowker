#pragma once

/* Generic */
#include "Games/ServerHandler.h"
#include "NetworkHandler/ServerPacketHandler.h"

/* STD */
#include <memory>
#include <vector>

namespace pla::common::games::dice_roller {

class DiceRollerServerHandler : public ServerHandler
{
public:
  explicit DiceRollerServerHandler(network::ServerPacketHandler& packetHandler);

private:
  bool _internalHandling() final;
};

} // namespaces
