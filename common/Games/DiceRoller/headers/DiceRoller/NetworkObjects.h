#pragma once

#include "Games/NetworkObjects.h"

namespace pla::common::games::dice_roller {

struct DiceRollerObjectSentToServer : public ObjectSentToServer
{
  bool wantToRollDice = true;
};


struct DiceRollerReplyFromServer : public ReplyFromServer
{
  bool turnAvailable = false;
};

} // namespaces
