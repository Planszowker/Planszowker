#pragma once

#include "Games/Objects.h"

namespace pla::common::games::dice_roller {

enum class DiceRollerRequestType : uint32_t {
  RollDice,
  Reroll
};

struct DiceRollerRequest : public Request
{
  DiceRollerRequestType type;
};


struct DiceRollerReply : public Reply
{
  uint8_t client;
  uint8_t dice[3];
};

} // namespaces
