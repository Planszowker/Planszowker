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
  explicit DiceRollerServerHandler(network::ServerPacketHandler& networkHandler);
  virtual ~DiceRollerServerHandler() = default;

  void run() final;
  void stop() final { };

  void networkCall(std::shared_ptr<sf::TcpSocket>& client, sf::Packet& packet, size_t playerId) final;
private:
  static constexpr size_t MaxPlayers = 2;
};

} // namespaces
