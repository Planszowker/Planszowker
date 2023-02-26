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
  virtual void getLobbyDetailsCallback(const std::any& arg) { };
  virtual void listOpenLobbiesCallback(const std::any& arg) { };
  virtual void joinLobbyCallback(const std::any& arg) { };
  // Increase callbacks if needed here...
protected:
  ICallbacks() = default;
};

}