#pragma once

#include "ICallbacks.h"
#include <Games/States/GameLobbyState.h>

#include <any>

namespace pla::games {

class GameLobbyState;

class GameLobbyCallbacks final : public ICallbacks
{
public:
  explicit GameLobbyCallbacks(GameLobbyState& state) : m_state(state) { }
  ~GameLobbyCallbacks() final = default;

  void getLobbyDetailsCallback(const std::any& arg) final;
  void listOpenLobbiesCallback(const std::any& arg) final;
  void joinLobbyCallback(const std::any& arg) final;
  void createLobbyCallback(const std::any& arg) final;
  void clientDisconnectedCallback(const std::any& arg) final;
private:
  GameLobbyState& m_state;
};


}