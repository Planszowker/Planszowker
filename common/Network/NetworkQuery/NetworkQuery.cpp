#include "NetworkQuery.h"

namespace pla::common::network {

bool NetworkQuery::queryClient(sf::TcpSocket &socket)
{
  // Send ACK? packet to client.
  // [TODO]: If client is still alive, there should be an acknowledgement.
  sf::Packet packet;
  sf::Socket::Status clientStatus;
  std::string ack = "ACK?";
  packet << ack;

  clientStatus = socket.send(packet);

  if (clientStatus != sf::Socket::Done) {
    return true;
  } else {
    return false;
  }
}

} // namespaces
