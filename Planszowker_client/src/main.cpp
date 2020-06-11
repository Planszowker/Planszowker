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

  sf::Packet packet;
  packet << "Siema packietowe ziuzielki :D";

  if(socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
    return -1;
  }

  sf::IpAddress server;
  cout << "Enter IP: ";
  cin >> server;

  unsigned short port = 54000;

  while(true) {
    if(socket.send(packet, server, port) != sf::Socket::Done) {
      return -2;
    }

    sf::IpAddress server_ip;
    unsigned short server_port;
    sf::Packet recvPacket;
    socket.receive(recvPacket, server_ip, server_port);

    string server_message;
    while(recvPacket >> server_message)
      cout << server_message;



    Sleep(1000);
  }

  return 0;
}
