#include <iostream>

#include "ErrorHandler/ExceptionThrower.h"
#include "GamesClient/Controller.h"

#include <SFML/Network.hpp>

#include <easylogging++.h>

using namespace pla::err_handler;
using namespace pla::games_client;

INITIALIZE_EASYLOGGINGPP

////////////
// CLIENT //
////////////
int main()
{
  el::Configurations customConf;
  customConf.setToDefault();
  customConf.set(el::Level::Debug, el::ConfigurationType::Format, "[%level]: %msg");
  el::Loggers::reconfigureLogger("default", customConf);

  sf::TcpSocket socket;

  // TODO: Change hard-coded IP and port
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
