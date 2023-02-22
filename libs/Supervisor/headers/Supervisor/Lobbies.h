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

  /*!
   * Remove lobby for given Client ID
   * @param creatorClientId Client ID to remove Lobby from
   */
  static void removeLobby(size_t creatorClientId);

  /*!
   * Get created lobby for
   * @param creatorClientId Lobby that client with given ID has created.
   * @return Pointer to lobby with given creator ID or nullptr otherwise.
   */
  [[maybe_unused]] [[nodiscard]]
  static Lobby* getLobby(size_t creatorClientId);

  /*!
   * Get all stored lobbies
   * @return Const reference to container with all lobbies
   */
  [[maybe_unused]] [[nodiscard]]
  static const std::unordered_map<size_t, Lobby>& getLobbies();
private:
  static std::unordered_map<size_t, Lobby> m_lobbies;
};

}