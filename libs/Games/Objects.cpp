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
