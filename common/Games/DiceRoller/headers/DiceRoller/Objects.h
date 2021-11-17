#pragma once

#include "Games/Objects.h"

namespace pla::common::games::dice_roller {

enum class DiceRollerRequestType : uint32_t {
  RollDice = 1,
  Reroll,
  Confirm,

  GetMyId
};

enum class DiceRollerReplyType : uint32_t {
  Invalid,
  ClientsInfo,
  GetIdReply,
  ReplyWithoutData,
  DiceReply,
};

/*
 * REQUEST: Client -> Server
 */
struct DiceRollerRequest : public Request
{
  DiceRollerRequestType type;
};

/*
 * REPLY: Server -> Client(s)
 */
struct DiceRollerReply : public Reply // General reply
{
  DiceRollerReplyType replyType = DiceRollerReplyType::Invalid;
};

// Specific reply with data
struct DiceRollerDiceReplyData {
  size_t clientId;
  uint8_t dice[3];
};

struct DiceRollerClientInfoData {
  std::vector<size_t> clientIds;
};

struct DiceRollerGetIdData {
  size_t clientId;
};

} // namespaces
