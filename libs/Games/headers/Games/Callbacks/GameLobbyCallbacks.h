#pragma once

#include <Callbacks/ICallbacks.h>
#include <States/GameLobbyState.h>

#include <any>

namespace pla::games {

class GameLobbyCallbacks final : public ICallbacks
{
public:
  explicit GameLobbyCallbacks(GameLobbyState& state) : m_state(state) { }
  ~GameLobbyCallbacks() final = default;

  void getLobbyDetailsCallback(const std::any& arg) final;
  void listOpenLobbiesCallback(const std::any& arg) final;
private:
  GameLobbyState& m_state;
};


}