#include "ClientPacketHandler.h"

#include "Games/Objects.h"
#include "TimeMeasurement/TimeLogger.h"
#include "CompilerUtils/FunctionInfoExtractor.h"
#include <chrono>

#include <iostream> // TODO: remove debug

using namespace pla::common::time_measurement;

namespace pla::common::network {

ClientPacketHandler::ClientPacketHandler(sf::TcpSocket& serverSocket)
  : PacketHandler()
  , m_serverSocket(serverSocket)
{
  m_serverSocket.setBlocking(false);
}


ClientPacketHandler::~ClientPacketHandler() {
  m_backgroundThread.join();
}


void ClientPacketHandler::runInBackground()
{
  m_run = true;

  std::thread backgroundThread{&ClientPacketHandler::_backgroundTask, this, std::ref(m_tcpSocketsMutex)};
  m_backgroundThread = std::move(backgroundThread);
}


void ClientPacketHandler::stop() {
  m_run = false;
}


void ClientPacketHandler::_backgroundTask(std::mutex& tcpSocketsMutex)
{
  /*
   * This method will receive and send packets to server
   */
  while(m_run) {
    std::this_thread::sleep_for(std::chrono::milliseconds (10));

    TimeLogger logger(GET_CURRENT_FUNCTION_NAME());
    const std::scoped_lock tcpSocketsLock(tcpSocketsMutex);

    // Receive packets from server
    sf::Packet receivePacket;
    sf::Socket::Status socketStatus = m_serverSocket.receive(receivePacket);
    while (socketStatus == sf::Socket::Partial) {
      socketStatus = m_serverSocket.receive(receivePacket);
    }

    if (socketStatus == sf::Socket::Done) {
      // Handle other packet type than HEARTBEAT
      //games::Reply reply = *reinterpret_cast<games::Reply*>(const_cast<void*>(receivePacket.getData()));
      games::Reply reply;
      receivePacket >> reply;

      if (reply.type == games::PacketType::GameSpecificData && reply.status == games::ReplyType::Success) {
        std::cout << "== Game Specific Reply ==\n";
        std::cout << "Reply from server: " << reply.body << "\n";
        std::cout << "Type: " << static_cast<int>(reply.type) << "\n";
        std::cout << "Status: " << static_cast<int>(reply.status) << "\n";
        std::cout << "== ======================\n";
      }

      // DEBUG PRINTOUT
      //if (reply.type == games::PacketType::GameSpecificData && reply.status == games::ReplyType::Success) {

      //}

      /*if (reply.status != games::PacketType::Heartbeat)
      {
        //std::cout << "Packet added to deque\n";
        m_receivedPackets.push_back(receivePacket);

        //std::cout << "Client packets size: " << m_receivedPackets.size() << std::endl;
      }*/
    }
  }
}

bool ClientPacketHandler::sendPacket(sf::Packet& packet) {
  TimeLogger logger(GET_CURRENT_FUNCTION_NAME());
  // Obtain mutex
  const std::scoped_lock tcpSocketsLock(m_tcpSocketsMutex);

  // Send packet to server
  sf::Socket::Status retStatus = m_serverSocket.send(packet);
  while (retStatus == sf::Socket::Partial) {
    retStatus = m_serverSocket.send(packet);
  }

  return (retStatus == sf::Socket::Done);
}

std::deque<sf::Packet>& ClientPacketHandler::getPackets() {
  TimeLogger logger(GET_CURRENT_FUNCTION_NAME());
  const std::scoped_lock tcpSocketsLock(m_tcpSocketsMutex);

  return m_receivedPackets;
}

void ClientPacketHandler::clearPackets() {
  TimeLogger logger(GET_CURRENT_FUNCTION_NAME());
  const std::scoped_lock tcpSocketsLock(m_tcpSocketsMutex);

  m_receivedPackets.clear();
}


} // namespaces
