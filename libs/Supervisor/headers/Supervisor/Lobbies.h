#pragma once

#include <Lobby.h>

#include <cstdlib>
#include <unordered_map>

namespace pla::supervisor {

class Lobbies{
public:
  /*!
   * Create new lobby
   * If a lobby for give Client ID has been already created, we have to overwrite it with new data.
   */
  static void createNewLobby(size_t creatorClientId, std::string lobbyName, std::string gameKey);
private:
  static std::unordered_map<size_t, Lobby> m_lobbies;
};

}