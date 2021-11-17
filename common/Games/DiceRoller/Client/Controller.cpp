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

    // Handle packets received from server
    const auto packets = m_clientPacketHandler.getPackets();
    for (const auto& packet: packets) {
      const void* data = packet.getData();

      DiceRollerReply reply = *reinterpret_cast<DiceRollerReply*>(const_cast<void*>(data));

      std::cout << "Reply type from server: " << static_cast<int>(reply.replyType) << "\n";

      // Desired action could not be performed
      if (reply.status == ReplyType::Invalid || reply.replyType == DiceRollerReplyType::Invalid) {
        //continue;
      }

      switch(reply.replyType) {
        case DiceRollerReplyType::DiceReply:
        {
          DiceRollerDiceReplyData diceReplyData = *reinterpret_cast<DiceRollerDiceReplyData *>(
                  static_cast<char *>(const_cast<void *>(data)) + sizeof(DiceRollerReply)
          );

          std::cout << "DiceReply " << diceReplyData.clientId << " with dice "
                    << static_cast<int>(diceReplyData.dice[0]) << " "
                    << static_cast<int>(diceReplyData.dice[1]) << " "
                    << static_cast<int>(diceReplyData.dice[2]) << "\n";

          break;
        }

        default:
          break;
      }

      m_clientPacketHandler.clearPackets();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds (10));
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
    requestPacket.append(reinterpret_cast<const void*>(&viewRequest), sizeof(viewRequest));
    m_clientPacketHandler.sendPacket(requestPacket);

  } catch (const std::bad_any_cast &e) {
    Logger::printDebug("Bad any cast in view's callback!");
    std::cout << e.what() << std::endl;
  }
}


} // namespace
