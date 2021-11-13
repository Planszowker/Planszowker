#include "Controller.h"

#include "DiceRoller/ConsoleViewCallbackObject.h"
#include "Logger/Logger.h"
#include "ErrorHandler/ErrorLogger.h"
#include "DiceRoller/Objects.h"
#include "NetworkHandler/ClientPacketHandler.h"

#include <thread>
#include <chrono>
#include <unordered_map>

using namespace pla::common::err_handler;
using namespace pla::common::logger;
using namespace pla::common;

namespace pla::common::games::dice_roller {

DiceRollerController::DiceRollerController(sf::TcpSocket& serverSocket)
  : Controller()
  , m_clientPacketHandler(serverSocket)
{
  m_view.init();
}


void DiceRollerController::run() {
  std::thread viewInput(&DiceRollerConsoleView::runLoop, m_view, this, std::ref(m_run));

  m_clientPacketHandler.runInBackground();

  while (m_run) {
    // TODO
    // Do nothing for now
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }

  viewInput.join();
}


void DiceRollerController::update()
{
  std::string updateString = "This is sample update string from controller. It will be received from server and/or model :)\n";

  m_view.update(std::make_any<std::string>(updateString));
}


void DiceRollerController::viewCallback(std::any& object) {
  try {
    auto viewRequest = std::any_cast<DiceRollerRequest>(object);

    Logger::printDebug("Callback from ConsoleView has occurred!");
    Logger::printDebug("Received data: " + std::to_string(static_cast<int>(viewRequest.type)));

    sf::Packet requestPacket;
    uint8_t type{2};
    requestPacket << type;
    requestPacket.append(reinterpret_cast<const void*>(&viewRequest), sizeof(viewRequest));
    m_clientPacketHandler.sendPacket(requestPacket);

  } catch (const std::bad_any_cast &e) {
    Logger::printDebug("Bad any cast in view's callback!");
    std::cout << e.what() << std::endl;
  }
}

} // namespace
