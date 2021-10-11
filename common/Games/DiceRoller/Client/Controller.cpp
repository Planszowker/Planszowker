#include "Controller.h"

#include "DiceRoller/ConsoleViewCallbackObject.h"
#include "Logger/Logger.h"
#include "ErrorHandler/ErrorLogger.h"
#include "DiceRoller/NetworkObjects.h"

#include <thread>
#include <chrono>

using namespace pla::common::err_handler;
using namespace pla::common::logger;

namespace pla::common::games::dice_roller {

DiceRollerController::DiceRollerController(sf::TcpSocket& serverSocket)
  : Controller(serverSocket)
{
  m_view.init();
}


void DiceRollerController::run() {
  std::thread viewInput(&DiceRollerConsoleView::runLoop, m_view, this, std::ref(m_runController));

  while (m_runController) {
    DiceRollerObjectSentToServer toServer;

    sf::Packet sendPacket;
    sendPacket.append(reinterpret_cast<void *>(&toServer), sizeof(toServer));
    Logger::printDebug("Size of toServer = " + std::to_string(sizeof(toServer)));

    // Send data to server
    sf::Socket::Status status = m_serverSocket.send(sendPacket);
    while (status == sf::Socket::Partial) {
      status = m_serverSocket.send(sendPacket);
    }
    if (status != sf::Socket::Done) {
      m_runController = false;
      viewInput.join();
      ErrorLogger::printError("Error sending data to server!");
      break;
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
  }

  viewInput.join();
}


void DiceRollerController::update()
{
  std::string updateString = "This is sample update string from controller. It will be received from server and/or model :)\n";

  m_view.update(std::make_any<std::string>(updateString));
}


void DiceRollerController::viewCallback(std::any& object)
{
  std::cout << "Ziuziu: " << object.type().name() << "\n";
  try {
    //auto callbackObject = std::any_cast<DiceRollerConsoleViewCallbackObject>(object);
    auto callbackObject = std::any_cast<bool>(object);

    if (callbackObject) {
      update();

      Logger::printDebug("Callback from ConsoleView has occurred!");
    } else {
      Logger::printDebug("Callback has occurred, but data got corrupted!");
    }
  } catch (const std::bad_any_cast& e) {
    Logger::printDebug("Bad any cast in view's callback!");
    std::cout << e.what() << std::endl;
  }
}


void DiceRollerController::receiveThread(std::mutex &mutex) {

}

} // namespace
