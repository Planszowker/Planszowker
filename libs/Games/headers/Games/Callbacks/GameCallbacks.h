#pragma once

#include <Games/Callbacks/ICallbacks.h>
#include <Games/States/GameState.h>

#include <any>

namespace pla::games {

class GameCallbacks final : public ICallbacks
{
public:
  GameCallbacks() = delete;
  explicit GameCallbacks(GameState& state) : m_state(state) { }
  ~GameCallbacks() final = default;

  void endTransactionCallback(const std::any& arg) final;

private:
  GameState& m_state;
};


}