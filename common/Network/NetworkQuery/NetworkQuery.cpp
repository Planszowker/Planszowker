#include "NetworkQuery.h"

namespace pla::common::network {

static bool queryClient(sf::TcpSocket& socket, std::map<size_t, common::client_info::ClientInfo>)
{
  // Send ACK? packet to client.
  // [TODO]: If client is still alive, there should be an acknowledgement.
  sf::Packet packet;
  sf::Socket::Status clientStatus;
  //std::string ack = "ACK?"; // [TODO]: Check if empty packet is supported
  //packet << ack;

  clientStatus = socket.send(packet);

  if (clientStatus != sf::Socket::Done) {
    return true;
  } else {
    return false;
  }
}

} // namespaces
