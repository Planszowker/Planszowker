#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

int main() {
  sf::Network::UdpSocket socket;

  if(!socket.bind(54000)) {
    // error
    return -1;
  }



  return 0;
}
