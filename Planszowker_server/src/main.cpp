#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>

#include "../../common/inc/ErrorLogger.hpp"

using namespace std;

using namespace Planszowker;

////////////
// SERVER //
////////////
int main() {

  sf::UdpSocket socket;
  socket.setBlocking(false);
  char data[100];
  std::size_t received;
  sf::IpAddress ip_sender;
  unsigned short port;

  Planszowker::common::ErrorLogger errlog;

  if(socket.bind(54000) != sf::Socket::Done) {
    return -1;
  }

  /*
  try {
    //throw MyException();
    throw std::domain_error("EXCEPTION KURWIU");
  }
  catch(MyException& e) {
    cout << e.what() << '\n';
    cout << "bubuubub\n";
  }
  //catch(std::exception& e){
    //cout << e.what() << '\n';
  //}
  */
  //errlog.printWarning("Ziuziuziuizuiz");
  //errlog.printError("ERROR");

  cout << sf::Socket::Done << " dla Done\n";
  cout << sf::Socket::NotReady << " dla NotReady\n";
  cout << sf::Socket::Partial << " dla Partial\n";
  cout << sf::Socket::Disconnected << " dla Disconnected\n";
  cout << sf::Socket::Error << " dla Error\n";

  int i = 0;

  while (true) {
    try {

      i++;
      if(i == 10)
        errlog.printWarning("takie se o ostrzezonko");
      else if (i == 20)
        errlog.printError("taki se o error krytyczny");

      cout << i << endl;

      sf::Socket::Status status = socket.receive(data, sizeof(data), received, ip_sender, port);
      if(status != sf::Socket::Done && status != sf::Socket::NotReady) {
        cout << status << '\n';
        return -2;
      }

      if(status == sf::Socket::NotReady)
        continue;

      std::cout << "Odebrano: " << data << " (" << received << ") od " << ip_sender << ":" << port << "\n";
    }
    catch (common::ExceptionThrower& e)
    {
      if (e.getPrio() == common::ExceptionPrio::Error)
      {
        cout << "Critical Error! Terminating process... \n";
        cout << "Error message: " << e.getMessage() << "\n";
        return EXIT_FAILURE;
      }
      else if (e.getPrio() == common::ExceptionPrio::Warning)
      {
        cout << "Received warning! Warning message: " << e.getMessage() << "\n";
      }
    }
  }
  return 0;
}
