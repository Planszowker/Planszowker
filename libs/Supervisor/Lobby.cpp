#include <Lobby.h>

namespace pla::supervisor {

Lobby::Lobby(size_t creatorClientId, std::string lobbyName, std::string gameKey)
  : m_creatorClientId(creatorClientId)
  , m_lobbyName(std::move(lobbyName))
  , m_gameKey(std::move(gameKey))
{
  m_clients.emplace_back(m_creatorClientId);

  _extractGameMetadata();
}


void Lobby::_extractGameMetadata()
{

}

}