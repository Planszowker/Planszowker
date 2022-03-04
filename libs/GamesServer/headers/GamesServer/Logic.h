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

/* ZIPIOS */
#include <zipios/zipfile.hpp>

namespace pla::games_server {

class Logic
{
public:
  using ClientIDsAndPointsMap = std::unordered_map<size_t, int>;

  Logic(std::vector<size_t>& clientIds, const std::string& gameName, network::ServerPacketHandler& packetHandler, zipios::ZipFile& zipFile);

  void handleGameLogic(size_t clientId, games::Request requestType);

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
  const std::vector<size_t>& _getClients() const { return m_clientsIDs; }
  int _getClientPoints(size_t clientID) const;

  const std::string& m_gameName;
  network::ServerPacketHandler& m_networkHandler;

  zipios::ZipFile& m_plaGameFile;

  std::vector<size_t>& m_clientsIDs;

  ClientIDsAndPointsMap m_clientsIDsAndPoints;
  ClientIDsAndPointsMap::iterator m_currentClientsIDAndPointsIt;

  size_t m_roundCounter{1};
  bool m_finished{false};

  sol::state m_luaVM;

  zipios::FileEntry::pointer_t m_boardEntry;
  zipios::FileEntry::pointer_t m_gameEntry;
  zipios::FileEntry::pointer_t m_initEntry;

  std::ostringstream m_boardScript;
  std::ostringstream m_initScript;
  std::ostringstream m_gameScript;
};

} // namespaces
