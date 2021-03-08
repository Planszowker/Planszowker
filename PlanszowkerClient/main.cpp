#include <iostream>

#include "ErrorHandler/ExceptionThrower.h"
#include "DiceRoller/Controller.h"
#include "DiceRoller/ConsoleView.h"

#include <SFML/Network.hpp>

using namespace pla::common::err_handler;
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

  try {
    DiceRollerController controller(socket);
    DiceRollerConsoleView consoleView;
    controller.attachView(&consoleView);

    controller.run();
  } catch (ExceptionThrower& e) {
    std::cout << "Exception " << e.what() << ": " << e.getMessage() << "\n";

  }

  return EXIT_SUCCESS;
}
