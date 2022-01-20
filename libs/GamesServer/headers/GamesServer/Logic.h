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

namespace pla::common::games::server {

class Logic
{
public:
  explicit Logic(std::vector<size_t>& clientIds, const std::string& gameName);

  void handleGameLogic(size_t clientId, Request requestType, network::ServerPacketHandler& packetHandler);

  [[nodiscard]] inline bool isGameFinished() const { return m_finished; }

private:
  [[nodiscard]] bool _checkIfTurnAvailable(size_t clientId) const;

  size_t m_currentTurnClientId;
  std::vector<size_t>& m_clientIds;

  bool m_finished;

  sol::state m_luaVM;

  const std::string& m_gameName;
};

} // namespaces
