#pragma once

#include <string>
#include <cstdint>

#include <SFML/Network.hpp>

namespace pla::common::games {

enum class PacketType : uint8_t
{
  Heartbeat, ///< Used for pinging clients.
  GameSpecificData, ///< Used for game specific data.
  ID ///< Used to get client's ID associated in server.
};


enum class ReplyType : uint8_t
{
  Invalid, ///< Invalid reply, received when some action cannot be performed.
  Success ///< Successful reply, queried action has been done successfully.
};


/*!
 * @brief Structure that is sent to server.
 */
struct Request
{
  PacketType type = PacketType::Heartbeat;
  std::string body;
};


/*!
 * @brief Structure that is received from server.
 */
struct Reply
{
  PacketType type = PacketType::GameSpecificData;
  ReplyType status = ReplyType::Invalid;
  std::string body;
};

} // namespaces

sf::Packet& operator << (sf::Packet& packet, const pla::common::games::Request& request);
sf::Packet& operator >> (sf::Packet& packet, pla::common::games::Request& request);
sf::Packet& operator << (sf::Packet& packet, pla::common::games::Reply& reply);
sf::Packet& operator >> (sf::Packet& packet, pla::common::games::Reply& reply);
