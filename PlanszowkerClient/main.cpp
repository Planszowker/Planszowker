#include <iostream>

#include "ErrorHandler/ExceptionThrower.h"
#include "GamesClient/Controller.h"

#include <SFML/Network.hpp>

using namespace pla::err_handler;
using namespace pla::games_client;

////////////
// CLIENT //
////////////
int main()
{
  sf::TcpSocket socket;

  // TODO: Change hard-coded IP
  sf::Socket::Status status = socket.connect("localhost", 27016);
  if(status != sf::Socket::Done) {
    return EXIT_FAILURE;
  }

  try {
    Controller controller(socket);

    controller.run();
  } catch (ExceptionThrower& e) {
    std::cout << "Exception " << e.what() << ": " << e.getMessage() << "\n";

  }

  return EXIT_SUCCESS;
}
