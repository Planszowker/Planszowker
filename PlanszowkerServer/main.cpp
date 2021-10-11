#include <SFML/Network.hpp>
#include <string>

#include "Games/ServerHandler.h"
#include "Games/ConsoleCharacters.h"
#include "DiceRoller/ServerHandler.h"
#include "ErrorHandler/ExceptionThrower.h"

using namespace pla::common;
using namespace pla::common::err_handler;
using namespace pla::common::games;
using namespace pla::common::games::dice_roller;

////////////
// SERVER //
////////////
int main() {

  sf::UdpSocket socket;
  socket.setBlocking(false);
  sf::Packet packet;
  sf::Packet server_packet;
  std::size_t received;
  sf::IpAddress ip_sender;
  unsigned short port;

  //std::cout << "Dice: " << diceString[Dice::One] << diceString[Dice::Two] << diceString[Dice::Three]
  //                      << diceString[Dice::Four] << diceString[Dice::Five] << diceString[Dice::Six] << "\n";

  //std::cout << "Ziuziely dwa \U0001f430\U0001f430\n";

  //ErrorLogger errorLogger;

  try {
    DiceRollerServerHandler serverHandler;

    serverHandler.run();

  } catch (ExceptionThrower& e) {
    std::cout << "Error message: " << e.getMessage() << "\n";
  }

  return EXIT_SUCCESS;
  /*
  //std::cout << " Version " << PLANSZOWKER_SERVER_VERSION_MAJOR << "." << PLANSZOWKER_SERVER_VERSION_MINOR << std::endl;


  // Try to create a socket for server, we will be connecting to this specific port,
  // so we need to know exactly
  try
  {
    if(socket.bind(54000) != sf::Socket::Done)
      errorLogger.printError("Socket was NOT created successfully! Aborting...");
  }
  catch (ExceptionThrower& e)
  {
    std::cout << "Critical Error! Terminating process... \n";
    std::cout << "Error message: " << e.getMessage() << "\n";
    return EXIT_FAILURE;
  }


  while (true)
  {
    try
    {

      sf::Socket::Status status = socket.receive(packet, ip_sender, port);
      if(status != sf::Socket::Done && status != sf::Socket::NotReady)
      {
        std::cout << status << '\n';
        return -2;
      }

      if(status == sf::Socket::NotReady)
        continue;

      std::string packetString;
      std::string out_string;
      if(packet >> packetString)
      {
        std::cout << "Odebrano: " << packetString << ") od " << ip_sender << ":" << port << "\n";
        server_packet.clear();
        server_packet << "Hello my friend from " << ip_sender.toString() << ":" << std::to_string(port) << ". I can hear you... \n";
        socket.send(server_packet, ip_sender, port);
      }
      else
        errorLogger.printError("Error reading packet!");
    }

    // Catch exceptions thrown by ErrorHandler
    // Used to terminate program with stack unwinding after critical error
    catch (ExceptionThrower& e)
    {
      // If we receive and error we should terminate the program
      // No std::exit() or std::shutdown() guarantee stack unwinding, to we need to use standard return from main
      if (e.getPrio() == ExceptionPrio::Error)
      {
        std::cout << "Critical Error! Terminating process... \n";
        std::cout << "Error message: " << e.getMessage() << "\n";
        return EXIT_FAILURE;
      }
      // If we received a warning, we should print out the message but not terminate the program
      else if (e.getPrio() == ExceptionPrio::Warning)
      {
        std::cout << "Received warning! Warning message: " << e.getMessage() << "\n";
      }
    }
  }
  */
  return EXIT_SUCCESS;
}
