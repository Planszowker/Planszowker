#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <windows.h>

using namespace std;

////////////
// CLIENT //
////////////
int main() {
  sf::UdpSocket socket;

  sf::Packet pakiet;
  sf::Uint32 ziu = 10203;
  pakiet << ziu;
  char data_to_send[] = "Siema ziuzielki :D";

  if(socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
    return -1;
  }

  sf::IpAddress server = "localhost";
  unsigned short port = 54000;

  while(true) {
    if(socket.send(data_to_send, sizeof(data_to_send), server, port) != sf::Socket::Done) {
      return -2;
    }

    cout << "Wysylam dane...\n";

    Sleep(1000);
  }

  return 0;
}
