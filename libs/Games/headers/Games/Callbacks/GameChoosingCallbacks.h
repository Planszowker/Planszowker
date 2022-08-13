#pragma once

#include "ICallbacks.h"

namespace pla::games {

class GameChoosingCallbacks final : public ICallbacks
{
public:
  GameChoosingCallbacks() = default;
  ~GameChoosingCallbacks() final = default;

  void IDCallback() final;
  void listAllAvailableGamesCallback() final;
};


}