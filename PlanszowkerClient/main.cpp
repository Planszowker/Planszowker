#include <iostream>

#include "ErrorHandler/ExceptionThrower.h"
#include "DiceRoller/Client.h"

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
    // Temporary solution as socket is not managed for now
    DiceRollerClient client{socket};

  } catch (ExceptionThrower& e) {
    std::cout << "Exception " << e.what() << ": " << e.getMessage() << "\n";

  }

  return EXIT_SUCCESS;
}
