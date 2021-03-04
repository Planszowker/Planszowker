#include "DiceRollerController.h"

#include "DiceRollerConsoleView.h"
#include "DiceRoller/ConsoleViewCallbackObject.h"
#include "Logger/Logger.h"
#include "ErrorHandler/ErrorLogger.h"
#include "Games/NetworkObjects.h"

#include <thread>
#include <chrono>

using namespace pla::common::err_handler;
using namespace pla::common::logger;

namespace pla::common::games::dice_roller {

DiceRollerController::DiceRollerController(sf::TcpSocket& serverSocket)
  : m_serverSocket(serverSocket)
  , m_view(nullptr)
{

}


void DiceRollerController::run()
{
  if (m_view == nullptr) {
    ErrorLogger::printError("View is null!");
    return;
  }

  // Initialize view - print some basic info
  m_view->init();

  std::thread viewInput(&DiceRollerConsoleView::runLoop, m_view, this);

  while (true)
  {
    if (m_view != nullptr) {
      ObjectSentToServer toServer;
      toServer.sentObject = std::make_any<bool>(true);

      sf::Packet sendPacket;
      sendPacket.append(reinterpret_cast<void*>(&toServer), sizeof(toServer));
      Logger::printDebug("Size of toServer = " + std::to_string(sizeof(toServer)));

      // Send data to server
      sf::Socket::Status status = m_serverSocket.send(sendPacket);
      while(status == sf::Socket::Partial) {
        status = m_serverSocket.send(sendPacket);
      }
      if (status != sf::Socket::Done) {
        ErrorLogger::printError("Error sending data to server!");
      }

      std::this_thread::sleep_for(std::chrono::seconds(2));
    }
  }

  viewInput.join();
}


void DiceRollerController::updateModel()
{
  // TODO
}


void DiceRollerController::updateView()
{
  std::string updateString = "This is sample update string from controller. It will be received from server and/or model :)\n";

  m_view->update(std::make_any<std::string>(updateString));
}


void DiceRollerController::viewCallback(std::any object)
{
  auto callbackObject = std::any_cast<DiceRollerConsoleViewCallbackObject>(object);

  if (callbackObject.occurred) {
    updateView();

    Logger::printDebug("Callback from ConsoleView has occurred!");
  } else {
    Logger::printDebug("Callback has occurred, but data got corrupted!");
  }
}


void DiceRollerController::getUpdatedModel()
{
  // TODO
}


void DiceRollerController::attachView(ConsoleView* view) {
  m_view = dynamic_cast<DiceRollerConsoleView*>(view);
}

} // namespace
