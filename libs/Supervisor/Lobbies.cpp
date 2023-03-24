#include <Lobbies.h>

#include <Games/Objects.h>

#include <utility>
#include <chrono>

#include <easylogging++.h>

namespace pla::supervisor {

std::unordered_map<size_t, Lobby> Lobbies::m_lobbies;
std::atomic<bool> Lobbies::m_runWatchdogThread {false};
std::mutex Lobbies::m_watchdogMutex;
std::thread Lobbies::m_watchdogThread;
utils::TickThread<std::chrono::seconds, 2> Lobbies::m_tickThread;

Lobby* Lobbies::createNewLobby(size_t creatorClientId, std::string lobbyName, std::string gameKey)
{
  std::scoped_lock lock(m_watchdogMutex);

  auto it = m_lobbies.find(creatorClientId);
  if (it == std::end(m_lobbies)) {
    // Lobby for given ClientID does NOT exist
    auto [newIt, inserted] = m_lobbies.insert({creatorClientId, {creatorClientId, std::move(lobbyName), std::move(gameKey)}});
    if (inserted) {
      return &(newIt->second);
    } else {
      return nullptr;
    }
  } else {
    // Lobby for given ClientID has been already created - overwrite it
    it->second = {creatorClientId, std::move(lobbyName), std::move(gameKey)};
    return &(it->second);
  }
}


Lobby* Lobbies::getLobby(size_t creatorClientId) {
  std::scoped_lock lock(m_watchdogMutex);

  auto it = m_lobbies.find(creatorClientId);
  if (it == m_lobbies.end()) {
    return nullptr;
  } else {
    return &(it->second);
  }
}


const std::unordered_map<size_t, Lobby>& Lobbies::getLobbies() {
  return m_lobbies;
}


void Lobbies::removeLobby(size_t creatorClientId) {
  std::scoped_lock lock(m_watchdogMutex);

  m_lobbies.erase(creatorClientId);
}


void Lobbies::_watchdogThread(network::SupervisorPacketHandler& packetHandler)
{
  constexpr int WatchdogSleepTime = 5;
  constexpr double WatchdogExceedMultiplier = 3.0;

  while (Lobbies::m_runWatchdogThread)
  {
    m_tickThread.waitForTick(); // Suspend current thread

    std::scoped_lock lock(m_watchdogMutex);

    // First of all, check if any created lobby has exceeded its time for heartbeat packets
    check_lobbies:
    for (const auto& [creatorClientId, lobby] : m_lobbies) {
      std::chrono::duration<double> lastResponseTime = std::chrono::steady_clock::now() - lobby.getLastResponseTime();
      if (lastResponseTime.count() > WatchdogExceedMultiplier * WatchdogSleepTime) {
        LOG(DEBUG) << "Lobby for client " << creatorClientId << " has exceeded watchdog time! Removing lobby...";

        _removeLobby(creatorClientId, packetHandler);
        goto check_lobbies;
      }
    }

    // Later, check if lobbies' clients have exceeded their time for heartbeat packets
    for (auto& [creatorClientId, lobby] : m_lobbies) {
      check_clients:
      for (const auto& [clientId, timestamp] : lobby.getClientsWithTimestamps()) {
        if (clientId == creatorClientId) {
          // We don't want to check client that is a creator of given lobby
          continue;
        }

        std::chrono::duration<double> lastResponseTime = std::chrono::steady_clock::now() - timestamp;
        if (lastResponseTime.count() > WatchdogExceedMultiplier * WatchdogSleepTime) {

          sf::Packet packet;
          games::Reply reply {
            .type = games::PacketType::DisconnectClient,
          };
          packet << reply;

          packetHandler.sendPacketToClient(clientId, packet);
          lobby.removeClient(clientId);
          lobby.sendUpdate(packetHandler);
          goto check_clients;
        }
      }
    }
  }
}


void Lobbies::startWatchdogThread(network::SupervisorPacketHandler& packetHandler)
{
  Lobbies::m_runWatchdogThread = true;
  Lobbies::m_watchdogThread = std::thread(&Lobbies::_watchdogThread, std::ref(packetHandler));
}


void Lobbies::stopWatchdogThread()
{
  Lobbies::m_runWatchdogThread = false;
  Lobbies::m_watchdogThread.join();
}


void Lobbies::updateLobbyLastResponseTime(size_t creatorClientId)
{
  std::scoped_lock lock(m_watchdogMutex);

  auto it = m_lobbies.find(creatorClientId);
  if (it != m_lobbies.end()) {
    it->second.updateLastResponseTime();
  }
}


void Lobbies::updateClientLastResponseTime(size_t creatorClientId, size_t clientId)
{
  std::scoped_lock lock(m_watchdogMutex);

  auto it = m_lobbies.find(creatorClientId);
  if (it != m_lobbies.end()) {
    auto& lobby = it->second;
    lobby.updateClientLastResponseTime(clientId);
  }
}


void Lobbies::_removeLobby(size_t creatorId, network::SupervisorPacketHandler& packetHandler) {
  // Non thread safe method to remove a lobby. Shouldn't be used outside Lobbies class.
  auto it = m_lobbies.find(creatorId);
  if (it != m_lobbies.end()) {
    auto& lobby = it->second;
    for (const auto& clientId : lobby.getClients()) {
      // Send ClientDisconnected reply to every connected client
      _sendDisconnect(clientId, packetHandler);
    }
    m_lobbies.erase(creatorId);
  }
}


void Lobbies::_sendDisconnect(size_t clientId, network::SupervisorPacketHandler& packetHandler) {
  sf::Packet packet;
  games::Reply reply {
    .type = games::PacketType::DisconnectClient,
  };
  packet << reply;

  packetHandler.sendPacketToClient(clientId, packet);
}

}