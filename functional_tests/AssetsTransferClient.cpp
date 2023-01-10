#include <SFML/Network.hpp>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <fstream>
#include <array>
#include <vector>

int main()
{
  sf::TcpSocket socket;
  socket.connect("localhost", 59595);

  {
    sf::Packet packet;
    packet << "Client-server communication is alive!";
    socket.send(packet);
  }

  std::ifstream textureStream {"../../planszowker_server/scripts/assets/DefaultBoard.jpg", std::ios::binary};

  auto buf = std::make_shared<std::vector<char>>(4096);
  sf::Packet packet;

  while (true) {
    textureStream.read(buf->data(), 4096);
    size_t count = textureStream.gcount();

    socket.send(buf->data(), count);
    std::cout << "Wysylam " << count << " bajtow...\n";

    if (count < 4096) {
      break;
    }
  }

  return EXIT_SUCCESS;
}