#pragma once

/* Generic */
//#include "Games/ServerHandler.h"
#include "NetworkHandler/ServerPacketHandler.h"
#include "Games/Objects.h"
#include "Rng/RandomGenerator.h"

/* SOL/LUA */
#include <sol/sol.hpp>

/* STD */
//#include <memory>
#include <vector>
#include <string>

namespace pla::common::games::server {

class Logic
{
public:
  using ClientIDsAndPointsMap = std::unordered_map<size_t, int>;

  explicit Logic(std::vector<size_t>& clientIds, const std::string& gameName);

  void handleGameLogic(size_t clientId, Request requestType, network::ServerPacketHandler& packetHandler);

  [[nodiscard]] inline bool isGameFinished() const { return m_finished; }

  [[nodiscard]] inline ClientIDsAndPointsMap getClientsMap() const { return m_clientsIDsAndPoints; }

private:
  [[nodiscard]] bool _checkIfTurnAvailable(size_t clientId) const;

  void _advanceRound();
  void _finishGame();
  void _addPointsToCurrentClient(int points);
  size_t _getRoundsCount() const { return m_roundCounter; }
  size_t _getCurrentClientID() const { return m_currentClientsIDAndPointsIt->first; }
  int _getCurrentPlayerPoints() const { return m_currentClientsIDAndPointsIt->second; }
  void _updateClients(std::string req) const;

  ClientIDsAndPointsMap m_clientsIDsAndPoints;
  ClientIDsAndPointsMap::iterator m_currentClientsIDAndPointsIt;

  size_t m_roundCounter{1};
  bool m_finished{false};

  sol::state m_luaVM;

  const std::string& m_gameName;

  static constexpr auto LUA_SCRIPT_GAMES_PREFIX = "lua-scripts/games/";
  static constexpr auto LUA_SCRIPT_GAMES_INIT_SUFFIX = "-init.lua";
};

} // namespaces
