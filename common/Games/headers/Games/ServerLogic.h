#pragma once

#include <list>

namespace pla::common::games {

class ServerLogic
{
public:
  explicit ServerLogic() : m_firstPlayer(false) { }

  /*!
   * @brief Adds player to players queue. Needed to make an order.
   */
  virtual void addPlayerId(std::size_t& playerId) = 0;

  /*!
   * @brief Checks if player that sends a query can roll a die.
   * @param playerId
   */
  virtual bool checkForAvailableMove(std::size_t& playerId) = 0;

protected:
  bool m_firstPlayer;
  std::list<std::size_t> m_playersQueue;
  std::list<std::size_t>::iterator m_currentPlayer;
};

} // namespaces
