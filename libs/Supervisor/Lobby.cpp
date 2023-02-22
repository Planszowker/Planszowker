#include <Lobby.h>

#include <GamesInfoExtractor.h>

#include <easylogging++.h>
#include <regex>

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
  static GamesInfoExtractor gamesInfoExtractor;
  const auto& plametas = gamesInfoExtractor.getPlametas();

  const std::regex plagameNameRegex {"([a-zA-Z0-9]+)(\\.plagame)"};

  std::smatch matches;
  for (const auto& [plametaGameFullKey, plametaParser]: plametas) {
    if (std::regex_search(plametaGameFullKey, matches, plagameNameRegex)) {
      // Extract game's name from file path - although spaces are permitted, they should not be used
      std::string gameShortKey = matches[1].str();

      if (gameShortKey == m_gameKey) {
        m_minPlayers = std::get<int>(plametaParser["settings:min_players"]->getVariant());
        m_maxPlayers = std::get<int>(plametaParser["settings:max_players"]->getVariant());
        LOG(DEBUG) << "We are dealing with " << gameShortKey << " game [min: " << m_minPlayers << ", max: " << m_maxPlayers << "]";
        return;
      }
    }
  }
}


void Lobby::addClient(size_t clientId)
{
  m_clients.emplace_back(clientId);
}


void Lobby::removeClient(size_t clientId)
{
  std::remove(m_clients.begin(), m_clients.end(), clientId);
}

}