#include "Controller.h"

#include "Logger/Logger.h"
#include "ErrorHandler/ErrorLogger.h"
#include "Games/Objects.h"
#include "NetworkHandler/ClientPacketHandler.h"
#include "Games/Characters.h"

#include <thread>
#include <chrono>
#include <unordered_map>
#include <sstream>

#include <nlohmann/json.hpp>

using namespace pla::common::err_handler;
using namespace pla::common::logger;
using namespace pla::common;

namespace pla::common::games::dice_roller {

DiceRollerController::DiceRollerController(sf::TcpSocket& serverSocket)
  : Controller()
  , m_clientPacketHandler(serverSocket)
{
  m_view.init();

  sf::Packet idPacket;

  Request request{
    .type = PacketType::ID
  };

  idPacket << request;

  m_clientPacketHandler.sendPacket(idPacket);
}


void DiceRollerController::run() {
  std::thread viewInput(&DiceRollerConsoleView::runLoop, m_view, this, std::ref(m_run));
  std::thread assetView(&DiceRollerConsoleView::showAssets, m_view);

  m_clientPacketHandler.runInBackground();

  //std::cout << "Return " << m_clientPacketHandler.getClientID(m_clientID) << "\n";

  do { } while (!m_clientPacketHandler.getClientID(m_clientID));

  while (m_run) {
    // TODO

    // Handle packets received from server
    const auto replies = m_clientPacketHandler.getReplies();
    for (const auto& reply: replies) {

      /*
      std::cout << "Reply: " << reply.body << "\n";
      std::cout << "Type: " << static_cast<int>(reply.type) << "\n";
      std::cout << "Status: " << static_cast<int>(reply.status) << "\n";
      std::cout << "== Controller Printout ==\n";
      */

      nlohmann::json j = nlohmann::json::parse(reply.body);

      for (auto pointsReply : j["PlayersInfo"]) {
        //std::cout << "I am client " << m_clientID << " and received ID is " << pointsReply["ID"] << "\n";
        std::stringstream ss;
        ss.str(pointsReply["ID"]);

        size_t replyClientID {0};
        ss >> replyClientID;

        if (replyClientID == m_clientID) {
          std::cout << "\nI have " << pointsReply["Points"] << " points.\n";
        }
      }

      for (auto event : j["Events"]) {
        std::string eventString = event["EventString"];
        eventString.erase(std::remove(eventString.begin(), eventString.end(), '\"'), eventString.end());
        std::cout << "\n" << eventString << "\n";
      }

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
    }

    std::this_thread::sleep_for(std::chrono::milliseconds (10));
  }

  viewInput.join();
  assetView.join();
}


void DiceRollerController::update()
{
  m_view.update(std::make_any<std::string>("Something..."));
}


void DiceRollerController::viewCallback(std::any& object) {
  try {
    auto viewRequest = std::any_cast<Request>(object);

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
