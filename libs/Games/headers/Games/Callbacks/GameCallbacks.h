#pragma once

#include "ICallbacks.h"
#include <Games/States/GameLobbyState.h>

#include <any>

namespace pla::games {

class GameCallbacks final : public ICallbacks
{
public:
  ~GameCallbacks() final = default;
};


}