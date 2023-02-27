#include <Lobby.h>

#include <GamesInfoExtractor.h>
#include <Games/Objects.h>

#include <nlohmann/json.hpp>
#include <easylogging++.h>
#include <regex>

namespace pla::supervisor {

Lobby::Lobby(size_t creatorClientId, std::string lobbyName, std::string gameKey)
  : m_creatorClientId(creatorClientId)
  , m_lobbyName(std::move(lobbyName))
  , m_gameKey(std::move(gameKey))
  , m_lastResponseTime(std::chrono::steady_clock::now())
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


bool Lobby::addClient(size_t clientId)
{
  if (m_clients.size() < m_maxPlayers) {
    // Check if ClientID is not already inserted
    if (std::find(m_clients.begin(), m_clients.end(), clientId) == std::end(m_clients)) {
      // If ClientID doesn't exist in the container, add it
      m_clients.emplace_back(clientId);
      return true;
    }
  }

  return false;
}


void Lobby::removeClient(size_t clientId)
{
  std::remove(m_clients.begin(), m_clients.end(), clientId);
}


void Lobby::sendUpdate(network::SupervisorPacketHandler& packetHandler) const
{
  sf::Packet packet;
  games::Reply reply {
    .type = games::PacketType::GetLobbyDetails,
    .status = games::ReplyType::Success,
  };

  nlohmann::json replyJson;
  replyJson["CreatorID"] = m_creatorClientId;
  replyJson["ClientIDs"] = m_clients;
  replyJson["LobbyName"] = m_lobbyName;
  replyJson["GameKey"] = m_gameKey;
  replyJson["Valid"] = true;
  reply.body = replyJson.dump();

  packet << reply;

  for (const auto& client: m_clients) {
    packetHandler.sendPacketToClient(client, packet);
  }
}

}