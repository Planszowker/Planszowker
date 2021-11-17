#include "ClientPacketHandler.h"

#include "Games/Objects.h"

#include <iostream> // TODO: remove debug

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
    const std::scoped_lock tcpSocketsLock(tcpSocketsMutex);

    // Receive packets from server
    sf::Packet receivePacket;
    sf::Socket::Status socketStatus = m_serverSocket.receive(receivePacket);
    while (socketStatus == sf::Socket::Partial) {
      socketStatus = m_serverSocket.receive(receivePacket);
    }

    if (socketStatus == sf::Socket::Done && receivePacket.getDataSize() >= sizeof(games::Reply)) {
      // Handle other packet type than HEARTBEAT
      games::Reply reply = *reinterpret_cast<games::Reply*>(const_cast<void*>(receivePacket.getData()));

      if (reply.packetType != games::PacketType::Heartbeat)
      {
        std::cout << "Packet added to deque\n";
        m_receivedPackets.push_back(receivePacket);

        std::cout << "Client packets size: " << m_receivedPackets.size() << std::endl;
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds (10));
  }
}

bool ClientPacketHandler::sendPacket(sf::Packet& packet) {
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
  const std::scoped_lock tcpSocketsLock(m_tcpSocketsMutex);

  return m_receivedPackets;
}

void ClientPacketHandler::clearPackets() {
  const std::scoped_lock tcpSocketsLock(m_tcpSocketsMutex);

  m_receivedPackets.clear();
}


} // namespaces
