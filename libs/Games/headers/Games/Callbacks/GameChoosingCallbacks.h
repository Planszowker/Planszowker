#pragma once

#include "ICallbacks.h"
#include "States/GameChoosingState.h"

#include <any>

namespace pla::games {

class GameChoosingCallbacks final : public ICallbacks
{
public:
  explicit GameChoosingCallbacks(GameChoosingState& state) : m_state(state) { }
  ~GameChoosingCallbacks() final = default;

  void listAllAvailableGamesCallback(const std::any& arg) final;
private:
  GameChoosingState& m_state;
};


}