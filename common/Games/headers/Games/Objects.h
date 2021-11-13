#pragma once

/* STD */
#include <any>

namespace pla::common::games {

enum class PacketType : uint8_t
{
  Heartbeat, ///< Used for pinging clients.
  GameSpecificData ///< Used for game specific data.
};


enum class ReplyType : uint8_t
{
  Invalid, ///< Invalid reply, received when some action cannot be performed
  Success ///< Successful reply, queried action has been done successfully.
};


/*!
 * @brief Structure that is sent to server.
 * It can contain anything inside, specific game implements its own object.
 */
struct Request
{
  PacketType packetType = PacketType::Heartbeat;
};


/*!
 * @brief Structure that is received from server.
 * This structure can be extended with any object, depending on game
 */
struct Reply
{
  /*!
   * Reply from server.
   */
  ReplyType status = ReplyType::Invalid;

  /*!
   * Additional string that can contain error string to be displayed.
   */
  std::string additionalInfo;
};

} // namespaces
