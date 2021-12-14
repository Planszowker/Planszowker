#include <iostream>

#include "ErrorHandler/ExceptionThrower.h"
#include "DiceRoller/Controller.h"
#include "DiceRoller/GraphicalView.h"

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
  std::string ipAddressString;
  std::cout << "Enter IP address: ";
  std::cin >> ipAddressString;
  std::cout << "Enter port: ";
  std::cin >> inPort;

  sf::IpAddress ipAddress(ipAddressString);

  sf::Socket::Status status = socket.connect(ipAddress, inPort);
  if(status != sf::Socket::Done) {
    return EXIT_FAILURE;
  }

  try {
    std::atomic_bool runThreads = true;
    sf::RenderWindow window{sf::VideoMode(800, 600), "Ziuziuziu"};
    DiceRollerGraphicalView view{window};
    DiceRollerController controller{socket, runThreads};

    controller.connectView(&view);

    std::thread controllerThread{&DiceRollerController::run, &controller};

    view.run();

    controllerThread.join();
  } catch (ExceptionThrower& e) {
    std::cout << "Exception " << e.what() << ": " << e.getMessage() << "\n";

  }

  return EXIT_SUCCESS;
}
