#pragma once

/* STD */
#include <any>

namespace pla::common::games {

enum class ServerReplies
{
  Invalid, ///< Invalid reply, received when some action cannot be performed
  Success ///< Successful reply, queried action has been done successfully.
};


/*!
 * @brief Structure that is sent to server.
 * It can contain anything inside, specific game implements its own object.
 */
struct ObjectSentToServer
{
};


/*!
 * @brief Structure that is received from server.
 * This structure can be extended with any object, depending on game
 */
struct ReplyFromServer
{
  /*!
   * Reply from server.
   */
  ServerReplies reply = ServerReplies::Invalid;

  /*!
   * Additional string that can contain error string to be displayed.
   */
  std::string additionalInfo;
};

} // namespaces
