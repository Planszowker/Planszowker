#pragma once

#include <any>

namespace pla::games {

/*
 * Callbacks interface
 * You can override only needed methods
 */
class ICallbacks
{
public:
  virtual ~ICallbacks() = default;

  virtual void IDCallback() { };
  virtual void listAllAvailableGamesCallback(std::any&) { };
  virtual void createLobbyCallback(std::any&) { };
  // Increase callbacks if needed here...
protected:
  ICallbacks() = default;
};

}