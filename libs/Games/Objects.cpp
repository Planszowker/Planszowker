#include "Objects.h"

using namespace pla::common::games;

sf::Packet& operator << (sf::Packet& packet, const Request& request)
{
  return packet << static_cast<uint8_t>(request.type) << request.body;
}

sf::Packet& operator >> (sf::Packet& packet, Request& request)
{
  std::underlying_type<PacketType>::type packetTypeType;
  packet >> packetTypeType;
  request.type = static_cast<PacketType>(packetTypeType);
  return packet >> request.body;
}

sf::Packet& operator << (sf::Packet& packet, pla::common::games::Reply& reply)
{
  return packet << static_cast<uint8_t>(reply.type) << static_cast<uint8_t>(reply.status) << reply.body;
}

sf::Packet& operator >> (sf::Packet& packet, pla::common::games::Reply& reply)
{
  std::underlying_type<PacketType>::type packetTypeType;
  packet >> packetTypeType;
  reply.type = static_cast<PacketType>(packetTypeType);

  std::underlying_type<ReplyType>::type replyTypeType;
  packet >> replyTypeType;
  reply.status = static_cast<ReplyType>(replyTypeType);

  return packet >> reply.body;
}
