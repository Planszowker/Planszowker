#include <iostream>

#include "DiceRoller/DiceRollerController.h"
#include "DiceRoller/DiceRollerConsoleView.h"

#include <SFML/Network.hpp>

using namespace pla::common::games::dice_roller;

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

  DiceRollerController controller(socket);
  DiceRollerConsoleView consoleView;
  controller.attachView(&consoleView);

  controller.run();

  return EXIT_SUCCESS;
}
