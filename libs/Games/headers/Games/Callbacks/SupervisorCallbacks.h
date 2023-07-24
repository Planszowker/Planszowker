#pragma once

#include "ICallbacks.h"

namespace pla::games {

class SupervisorCallbacks final : public ICallbacks
{
public:
  SupervisorCallbacks() = default;
  ~SupervisorCallbacks() = default;

  void IDCallback() final;
  void listAllAvailableGamesCallback() final;
};

}