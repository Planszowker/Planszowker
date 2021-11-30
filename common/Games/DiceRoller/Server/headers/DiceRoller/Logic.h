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

  inline bool isGameFinished() { return m_finished; }

private:
  bool _checkIfTurnAvailable(size_t clientId);

  size_t m_currentTurnId;
  std::vector<size_t>& m_clientIds;

  rng::RandomGenerator<uint8_t> m_rngGenerator;

  std::array<uint8_t, 3> m_dice;

  State m_state;

  std::unordered_map<size_t, size_t> m_playerPoints;

  size_t m_rounds;

  constexpr static size_t MaxRounds = 3;

  bool m_finished;
};

} // namespaces
