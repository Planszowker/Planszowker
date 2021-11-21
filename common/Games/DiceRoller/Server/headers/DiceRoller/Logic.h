#pragma once

/* Generic */
//#include "Games/ServerHandler.h"
#include "NetworkHandler/ServerPacketHandler.h"
#include "DiceRoller/Objects.h"
#include "Rng/RandomGenerator.h"

/* STD */
//#include <memory>
#include <vector>

namespace pla::common::games::dice_roller {

enum class State {
  Entry,
  Rolled,
  Rerolled,
  Confirmed
};

class DiceRollerLogic
{
public:
  explicit DiceRollerLogic(std::vector<size_t>& clientIds);

  void handleGameLogic(size_t clientId, DiceRollerRequestType requestType, network::ServerPacketHandler& packetHandler);

private:
  bool _checkIfTurnAvailable(size_t clientId);

  size_t m_currentTurnId;
  std::vector<size_t>& m_clientIds;

  rng::RandomGenerator<uint8_t> m_rngGenerator;

  State m_state;

  std::unordered_map<size_t, size_t> m_playerPoints;
};

} // namespaces
