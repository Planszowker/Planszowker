#include <iostream>
#include <chrono>
#include <thread>

#include <SFML/Network.hpp>

////////////
// CLIENT //
////////////
int main()
{
  sf::TcpSocket socket;

  unsigned short inPort = 0;
  std::cin >> inPort;

  sf::Socket::Status status = socket.connect("localhost", inPort);
  if(status != sf::Socket::Done) {
    return EXIT_FAILURE;
  }

  for(;;) {
    std::cout << "Jestem sobie klient :)\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
  };

  return EXIT_SUCCESS;
}
