#pragma once

#include <NetworkHandler/SupervisorPacketHandler.h>

#include <Games/Objects.h>

#include <string>
#include <vector>
#include <utility>
#include <chrono>

namespace pla::supervisor {

class Lobby
{
public:
  using Clock = std::chrono::steady_clock;
  using TimePoint = std::chrono::time_point<Clock>;
  using ClientContainer = std::unordered_map<size_t, TimePoint>;

  Lobby() = delete;
  Lobby(size_t creatorClientId, std::string lobbyName, std::string gameKey);

  bool addClient(size_t clientId);
  void removeClient(size_t clientId);

  [[maybe_unused]] [[nodiscard]]
  size_t getCreatorClientId() const { return m_creatorClientId; }

  [[maybe_unused]] [[nodiscard]]
  std::string_view getLobbyName() const { return m_lobbyName; };

  [[maybe_unused]] [[nodiscard]]
  std::string_view getGameKey() const { return m_gameKey; };

  [[maybe_unused]] [[nodiscard]]
  std::vector<size_t> getClients() const;

  [[maybe_unused]] [[nodiscard]]
  ClientContainer getClientsWithTimestamps() const { return m_clients; }

  [[maybe_unused]] [[nodiscard]]
  int getMinPlayers() const { return m_minPlayers; }

  [[maybe_unused]] [[nodiscard]]
  int getMaxPlayers() const { return m_maxPlayers; }

  [[maybe_unused]] [[nodiscard]]
  int getCurrentPlayers() const { return static_cast<int>(m_clients.size()); }

  [[maybe_unused]] [[nodiscard]]
  std::chrono::time_point<std::chrono::steady_clock> getLastResponseTime() const { return m_lastResponseTime; }

  [[maybe_unused]] [[nodiscard]]
  bool hasEnoughClients() const { return m_clients.size() >= m_minPlayers; }

  void updateLastResponseTime() { m_lastResponseTime = std::chrono::steady_clock::now(); }

  /*!
   * Send update to every Client connected to specific lobby.
   * All Clients connected to given lobby will receive new JSON
   * with details about the lobby.
   *
   * @param packetHandler Supervisor Packet Handler used to send details over network.
   */
  void sendUpdate(network::SupervisorPacketHandler& packetHandler) const;

  void sendToAllClients(network::SupervisorPacketHandler& packetHandler, games::PacketType type, const std::string& body);

  /*!
   * Update Client last response time.
   * Last response time is used in watchdog thread to check if user still wants to be in lobby.
   *
   * @param
   */
  void updateClientLastResponseTime(size_t clientId);
private:
  void _extractGameMetadata();
  size_t m_creatorClientId;
  std::string m_lobbyName;
  std::string m_gameKey;
  int m_minPlayers;
  int m_maxPlayers;

  ClientContainer m_clients;
  TimePoint m_lastResponseTime; ///< Last time receiving lobby heartbeat from Client.
};

}