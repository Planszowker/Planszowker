#include <SFML/Network.hpp>
#include <string>

#include "GamesServer/ServerHandler.h"
#include "ErrorHandler/ExceptionThrower.h"
#include "GamesServer/Logic.h"

#include <ErrorHandler/ExceptionThrower.h>

#include <ServerSupervisor/ServerSupervisor.h>

#include <easylogging++.h>

#include <fstream>

#define ELPP_THREAD_SAFE // Set multithreading for EasyLogging

using namespace pla::server;
using namespace pla::common;
using namespace pla::common::err_handler;
using namespace pla::common::games::server;

INITIALIZE_EASYLOGGINGPP

static constexpr auto CONFIG_FILENAME = "config.plameta";

////////////
// SERVER //
////////////
int main() {

  std::ifstream configStream {CONFIG_FILENAME};

  el::Configurations customConf;
  customConf.setToDefault();
  customConf.set(el::Level::Debug, el::ConfigurationType::Format, "[%level]: %msg");
  el::Loggers::reconfigureLogger("default", customConf);

  if (!configStream.is_open()) {
    LOG(ERROR) << "Cannot open " << CONFIG_FILENAME << " file!";
    return EXIT_FAILURE;
  }

  try {
    ServerSupervisor serverSupervisor {configStream};
    serverSupervisor.run();
  } catch (ExceptionThrower& e) {
    LOG(INFO) << "Terminating Planszówker Server due to errors...";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;

  sf::TcpSocket socket;
  socket.setBlocking(false);
  sf::Packet packet;
  sf::Packet server_packet;
  std::size_t received;
  sf::IpAddress ip_sender;
  unsigned short port;

  try {
    network::ServerPacketHandler serverPacketHandler{2};
    ServerHandler serverHandler("DiceRoller", serverPacketHandler);

    serverHandler.run();

  } catch (ExceptionThrower& e) {
    std::cout << "Error message: " << e.getMessage() << "\n";
  }

  return EXIT_SUCCESS;
}
