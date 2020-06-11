#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>

#include "ErrorLogger.hpp"

using namespace std;

using namespace Planszowker;

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

  common::ErrorLogger errorLogger;


  // Try to create a socket for server, we will be connecting to this specific port,
  // so we need to know exactly **
  try
  {
    if(socket.bind(54000) != sf::Socket::Done)
      errorLogger.printError("Socket was NOT created successfully! Aborting...");
  }
  catch (common::ExceptionThrower& e)
  {
    cout << "Critical Error! Terminating process... \n";
    cout << "Error message: " << e.getMessage() << "\n";
    return EXIT_FAILURE;
  }


  while (true)
  {
    try
    {

      sf::Socket::Status status = socket.receive(packet, ip_sender, port);
      if(status != sf::Socket::Done && status != sf::Socket::NotReady)
      {
        cout << status << '\n';
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

    // Catch exceptions thrown by ErrorLogger
    // Used to terminate program with stack unwinding after critical error
    catch (common::ExceptionThrower& e)
    {
      // If we receive and error we should terminate the program
      // No std::exit() or std::shutdown() guarantee stack unwinding, to we need to use standard return from main
      if (e.getPrio() == common::ExceptionPrio::Error)
      {
        cout << "Critical Error! Terminating process... \n";
        cout << "Error message: " << e.getMessage() << "\n";
        return EXIT_FAILURE;
      }
      // If we received a warning, we should print out the message but not terminate the program
      else if (e.getPrio() == common::ExceptionPrio::Warning)
      {
        cout << "Received warning! Warning message: " << e.getMessage() << "\n";
      }
    }
  }
  return 0;
}
