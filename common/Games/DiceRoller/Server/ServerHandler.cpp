#include "ServerHandler.h"

#include "DiceRoller/NetworkObjects.h"
#include "ErrorHandler/ErrorLogger.h"

namespace pla::common::games::dice_roller {

using namespace err_handler;

DiceRollerServerHandler::DiceRollerServerHandler()
{
  m_running = true;
}


void DiceRollerServerHandler::run()
{
  m_networkHandler.run();
}


void DiceRollerServerHandler::networkCall(std::shared_ptr<sf::TcpSocket>& client, sf::Packet& packet, size_t playerId)
{
  if (client == nullptr) {
    ErrorLogger::printError("Client\'s socket is a nullptr!");
  }


  const std::size_t packetDataSize = packet.getDataSize();

  // Get sent data
  auto receivedObject = *reinterpret_cast<DiceRollerObjectSentToServer*>(const_cast<void*>(packet.getData()));

  // Debug -> TODO: Remove
  std::cout << std::boolalpha << "Received: " << receivedObject.wantToRollDice << " from playerID: " << playerId <<  std::endl;

  // Sample data. TODO: Change it later
  DiceRollerReplyFromServer reply;
  reply.additionalInfo = "It is not your turn!";
  reply.turnAvailable = true;
  reply.reply = ServerReplies::Invalid;

  sf::Packet sendPacket;
  sendPacket.append(reinterpret_cast<void*>(&reply), sizeof(reply));

  // Send data to client
  sf::Socket::Status status = client->send(sendPacket);
  while(status == sf::Socket::Partial) {
    status = client->send(sendPacket);
  }

  if (status != sf::Socket::Done) {
    ErrorLogger::printError("Error sending data to server!");
    return;
  }
}

} // namespaces
