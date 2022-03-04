#include <iostream>

#include "ErrorHandler/ExceptionThrower.h"
#include "GamesClient/Controller.h"
#include "GamesClient/ConsoleView.h"

#include <SFML/Network.hpp>

using namespace pla::err_handler;
using namespace pla::common::games::dice_roller;

////////////
// CLIENT //
////////////
int main()
{
  sf::TcpSocket socket;

  sf::Socket::Status status = socket.connect("localhost", 27016);
  if(status != sf::Socket::Done) {
    return EXIT_FAILURE;
  }

  try {
    DiceRollerController controller(socket);

    controller.run();
  } catch (ExceptionThrower& e) {
    std::cout << "Exception " << e.what() << ": " << e.getMessage() << "\n";

  }

  return EXIT_SUCCESS;
}
