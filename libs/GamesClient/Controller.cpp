#include "Controller.h"

#include "Logger/Logger.h"
#include "ErrorHandler/ErrorLogger.h"
#include "Games/Objects.h"
#include "NetworkHandler/ClientPacketHandler.h"
#include "Games/Characters.h"

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

//  sf::Packet idPacket;
//
//  Request request{
//    .type = PacketType::GameSpecificData;
//  };
//
//  idPacket.append(&request, sizeof(request));
//
//  m_clientPacketHandler.sendPacket(idPacket);
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

//      DiceRollerReply reply = *reinterpret_cast<DiceRollerReply*>(const_cast<void*>(data));
//
//      //std::cout << "Reply type from server: " << static_cast<int>(reply.replyType) << "\n";
//
//      // Desired action could not be performed
//      if (reply.status == ReplyType::Invalid || reply.replyType == DiceRollerReplyType::Invalid) {
//        continue;
//      }
//
//      switch(reply.replyType) {
//        case DiceRollerReplyType::DiceReply:
//        {
//          // TODO: This casts are awful... Figure out better way to do this.
//          DiceRollerDiceReplyData diceReplyData = *reinterpret_cast<DiceRollerDiceReplyData *>(
//                  static_cast<char *>(const_cast<void *>(data)) + sizeof(DiceRollerReply)
//          );
//
//          std::cout << "DiceReply " << diceReplyData.clientId << " with dice "
//                    << diceString[static_cast<int>(diceReplyData.dice[0]) - 1] << " "
//                    << diceString[static_cast<int>(diceReplyData.dice[1]) - 1] << " "
//                    << diceString[static_cast<int>(diceReplyData.dice[2]) - 1] << "\n";
//
//          break;
//        }
//
//        case DiceRollerReplyType::GetIdReply:
//        {
//          //std::cout << "GetIdReply reply type...\n";
//
//          auto getIdData = *reinterpret_cast<DiceRollerGetIdData *>(
//                  static_cast<char *>(const_cast<void *>(data)) + sizeof(DiceRollerReply)
//          );
//
//          std::cout << "MyID is " << getIdData.clientId << "\n";
//
//          break;
//        }
//
//        case DiceRollerReplyType::PointsReply:
//        {
//          //std::cout << "Points reply type...\n";
//
//          auto pointsReplyData = *reinterpret_cast<DiceRollerPointsReplyData *>(
//                  static_cast<char *>(const_cast<void *>(data)) + sizeof(DiceRollerReply)
//          );
//
//          std::cout << "=ROUND " << pointsReplyData.currentRound << "= Client(" << pointsReplyData.clientId << ") has total of " << pointsReplyData.points << " points." << "\n";
//
//          break;
//        }
//
//        case DiceRollerReplyType::FinishReply:
//        {
//          //std::cout << "Points reply type...\n";
//
//          auto finishReplyData = *reinterpret_cast<DiceRollerFinishReplyData *>(
//                  static_cast<char *>(const_cast<void *>(data)) + sizeof(DiceRollerReply)
//          );
//
//          if (finishReplyData.draw) {
//            std::cout << "=== GAME FINISHED WITH DRAW ===\n";
//          } else {
//            std::cout << "=== GAME FINISHED: PLAYER " << finishReplyData.winnerClientId << " HAS WON WITH " << finishReplyData.points << "! ===\n";
//          }
//
//          break;
//        }
//
//        default:
//          break;
//      }

      m_clientPacketHandler.clearPackets();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds (10));
  }

  viewInput.join();
}


void DiceRollerController::update()
{
  m_view.update(std::make_any<std::string>("Something..."));
}


void DiceRollerController::viewCallback(std::any& object) {
  try {
    Request viewRequest = std::any_cast<Request>(object);

    //Logger::printDebug("Callback from ConsoleView has occurred!");
    //Logger::printDebug("Received data: " + std::to_string(static_cast<int>(viewRequest.type)));

    sf::Packet requestPacket;
    requestPacket << viewRequest;
    m_clientPacketHandler.sendPacket(requestPacket);

  } catch (const std::bad_any_cast &e) {
    //Logger::printDebug("Bad any cast in view's callback!");
    std::cout << e.what() << std::endl;
  }
}


} // namespace
