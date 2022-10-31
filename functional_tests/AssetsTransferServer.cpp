#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

int main()
{
  sf::TcpListener listener;
  listener.listen(59595);

  sf::TcpSocket clientSocket;
  listener.accept(clientSocket);

  clientSocket.setBlocking(true);

  {
    std::cout << "Receiving string...\n";
    sf::Packet packet;
    clientSocket.receive(packet);
    std::string packetStr;
    packet >> packetStr;
    std::cout << packetStr << "\n";
  }

  std::cout << "Receiving texture...\n";
  auto recvBuf = std::make_shared<std::vector<char>>(4096);
  auto buf = std::make_shared<std::vector<char>>();
  size_t recv;

  while (clientSocket.receive(recvBuf->data(), 4096, recv) == sf::Socket::Done) {
    buf->insert(buf->end(), recvBuf->data(), recvBuf->data() + recv);
    std::cout << "Odbieram " << recv << " bajtow...\n";

    if (recv < 4096) {
      break;
    }
  }

  std::cout << buf->size() << "\n";

  std::shared_ptr<sf::Texture> t = std::make_shared<sf::Texture>();
  t->loadFromMemory(buf->data(), buf->size());

  std::cout << t->getSize().x << " x " << t->getSize().y << "\n";

  sf::RenderWindow window(sf::VideoMode(800, 600), "Ziuziu");

  sf::Sprite sprite;
  sprite.setTexture(*t);

  while (window.isOpen()) {
    sf::Event event{};
    while(window.pollEvent(event)){
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear(sf::Color::Black);
    window.draw(sprite);
    window.display();
  }

  return EXIT_SUCCESS;
}