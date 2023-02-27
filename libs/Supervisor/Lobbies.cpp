#include <Lobbies.h>

#include <utility>
#include <chrono>

#include <easylogging++.h>

namespace pla::supervisor {

std::unordered_map<size_t, Lobby> Lobbies::m_lobbies;
std::atomic<bool> Lobbies::m_runWatchdogThread {false};
std::mutex Lobbies::m_watchdogMutex;
std::thread Lobbies::m_watchdogThread;

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

  auto it = m_lobbies.find(creatorClientId);
  if (it != m_lobbies.end()) {
    m_lobbies.erase(it);
  }
}


void Lobbies::_watchdogThread()
{
  constexpr int WatchdogSleepTime = 5;
  constexpr double WatchdogExceedMultiplier = 3.0;

  while (Lobbies::m_runWatchdogThread)
  {
    std::this_thread::sleep_for(std::chrono::seconds(WatchdogSleepTime));

    std::scoped_lock lock(m_watchdogMutex);

    for (const auto& [creatorClientId, lobby] : m_lobbies) {
      std::chrono::duration<double> lastResponseTime = std::chrono::steady_clock::now() - lobby.getLastResponseTime();
      if (lastResponseTime.count() > WatchdogExceedMultiplier * WatchdogSleepTime) {
        LOG(DEBUG) << "Lobby for client " << creatorClientId << " has exceeded watchdog time! Removing lobby...";

        auto it = m_lobbies.find(creatorClientId);
        if (it != m_lobbies.end()) {
          m_lobbies.erase(it);
        }
      }
    }
  }
}


void Lobbies::startWatchdogThread()
{
  Lobbies::m_runWatchdogThread = true;
  static auto _thread = std::thread(&Lobbies::_watchdogThread);
  Lobbies::m_watchdogThread = std::move(_thread);
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

}