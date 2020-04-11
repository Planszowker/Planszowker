#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>

int main() {

  sf::UdpSocket socket;
  char data[100];
  std::size_t received;
  sf::IpAddress ip_sender;
  unsigned short port;

  if(socket.bind(54000) != sf::Socket::Done) {
    return -1;
  }

  if(socket.receive(data, sizeof(data), received, ip_sender, port) != sf::Socket::Done) {
    return -2;
  }

  std::cout << "Odebrano: " << data << " (" << received << ") od " << ip_sender << ":" << port << "\n";

  return 0;
}
