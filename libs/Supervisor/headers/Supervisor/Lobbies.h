#pragma once

#include <Lobby.h>

#include <cstdlib>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <atomic>

namespace pla::supervisor {

class Lobbies{
public:
  /*!
   * Create new lobby.
   * If a lobby for give Client ID has been already created, we have to overwrite it with new data.
   *
   * @return Pointer to newly created lobby.
   */
  static Lobby* createNewLobby(size_t creatorClientId, std::string lobbyName, std::string gameKey);

  /*!
   * Remove lobby for given Client ID.
   *
   * @param creatorClientId Client ID to remove Lobby from
   */
  static void removeLobby(size_t creatorClientId);

  /*!
   * Get created lobby for given Creator ID.
   *
   * @param creatorClientId Lobby that client with given ID has created.
   * @return Pointer to lobby with given creator ID or nullptr otherwise.
   */
  [[maybe_unused]] [[nodiscard]]
  static Lobby* getLobby(size_t creatorClientId);

  /*!
   * Get all stored lobbies.
   *
   * @return Const reference to container with all lobbies
   */
  [[maybe_unused]] [[nodiscard]]
  static const std::unordered_map<size_t, Lobby>& getLobbies();

  /*!
   * Start watchdog thread.
   * It is used to check if we have received heartbeat from Creator ID. If specified time is exceeded,
   * we remove given lobby.
   */
  static void startWatchdogThread();

  /*!
   * Stop watchdog thread.
   * It causes current thread to wait until watchdog thread terminates.
   */
  static void stopWatchdogThread();

  /*!
   * Update last response time for given lobby in thread safety manner.
   *
   * @param CreatorClientID Lobby's Creator ID to update last response time.
   */
  static void updateLobbyLastResponseTime(size_t creatorClientId);
private:
  static std::unordered_map<size_t, Lobby> m_lobbies;

  static std::atomic<bool> m_runWatchdogThread;
  static std::mutex m_watchdogMutex;
  static void _watchdogThread();

  static std::thread m_watchdogThread;
};

}