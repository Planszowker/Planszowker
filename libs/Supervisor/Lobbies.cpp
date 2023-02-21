#include <Lobbies.h>

#include <utility>

namespace pla::supervisor {

std::unordered_map<size_t, Lobby> Lobbies::m_lobbies;

void Lobbies::createNewLobby(size_t creatorClientId, std::string lobbyName, std::string gameKey)
{
  auto it = m_lobbies.find(creatorClientId);
  if (it == std::end(m_lobbies)) {
    // Lobby for given ClientID does NOT exist
    m_lobbies.insert({creatorClientId, {creatorClientId, std::move(lobbyName), std::move(gameKey)}});
  } else {
    // Lobby for given ClientID has been already created - overwrite it
    it->second = {creatorClientId, std::move(lobbyName), std::move(gameKey)};
  }
}

}