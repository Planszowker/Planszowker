#include "ServerLogic.h"

#include <algorithm>

#include <iostream> // Debug

namespace pla::common::games::dice_roller {

void DiceRollerServerLogic::addPlayerId(std::size_t& playerId)
{
  auto iterator = std::find(m_playersQueue.begin(), m_playersQueue.end(), playerId);
  if (iterator == m_playersQueue.end()) {
    m_playersQueue.push_back(playerId);

    if (!m_firstPlayer) {
      m_currentPlayer = (--iterator);
      std::cout << "m_currentPlayer: " << *iterator << std::endl;
      m_firstPlayer = true;
    }
  }
}


bool DiceRollerServerLogic::checkForAvailableMove(std::size_t &playerId)
{
  return false;
}

} // namespaces
