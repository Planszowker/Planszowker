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

  void downloadAssetsCallback(const std::any& arg) final;

  void gameSpecificDataCallback(const std::any& arg) final;

private:
  GameState& m_state;
};


}