#pragma once

#include <any>
#include <nlohmann/json.hpp>

namespace pla::games {

/*
 * Callbacks interface
 * You can override only needed methods
 */
class ICallbacks
{
public:
  virtual ~ICallbacks() = default;

  void IDCallback(std::any& arg);
  virtual void listAllAvailableGamesCallback(const std::any&) { };
  virtual void createLobbyCallback(const std::any&) { };
  virtual void getLobbyDetailsCallback(const std::any&) { };
  virtual void listOpenLobbiesCallback(const std::any&) { };
  virtual void joinLobbyCallback(const std::any&) { };
  virtual void disconnectClientCallback(const std::any&) { };
  virtual void startGameCallback(const std::any&) { };
  virtual void downloadAssetsCallback(const std::any&) { };
  virtual void gameSpecificDataCallback(const std::any&) { };
  // Increase callbacks if needed here...
protected:
  ICallbacks() = default;
};

}