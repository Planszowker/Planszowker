#include "ServerHandler.h"
#include "TimeMeasurement/TimeLogger.h"
#include "CompilerUtils/FunctionInfoExtractor.h"
#include "Games/Objects.h"
#include "GamesHandler.h"

namespace pla::common::games::server {

void ServerHandler::run()
{
  m_packetHandler.runInBackground();

  while(m_run)
  {
    if (!_internalHandling()) {
      break;
    }
  }

  m_packetHandler.stop();
}


bool ServerHandler::_internalHandling() {
  time_measurement::TimeLogger timeLogger(GET_CURRENT_FUNCTION_NAME());

  std::vector<size_t> keys;
  auto packetsMap = m_packetHandler.getPackets(keys);

  // TODO: Not elegant, maybe better solution?
  for (const auto& key: keys) {
    // Find map entry with client ID -> (size_t, deque) is received
    const auto mapIt = packetsMap.find(key);
    if (mapIt == packetsMap.end()) {
      continue;
    }

    std::cout << "DEBUG" << std::endl;
    // Iterate over all packets in deque
    for (auto& packet: mapIt->second) {
      Request request{};
      packet >> request;

      if (request.type == PacketType::Heartbeat) {
        // We don't care about Heartbeat packets
        continue;
      } else if (request.type == PacketType::ID) {
        // If we get ID request, we need to send client's ID
        Reply idReply{
          .type = PacketType::ID,
          .status = ReplyType::Success,
          .body = std::to_string(key)
        };
        sf::Packet replyPacket;
        replyPacket << idReply;

        m_packetHandler.sendPacketToClient(key, replyPacket);
        continue;
      }

      std::cout << "Type: " << static_cast<int>(request.type) << "\n";
      std::cout << "Body: " << request.body << "\n";

      if (m_packetHandler.hasEnoughClientsConnected()) {
        static GamesHandler gamesHandler {keys, m_gameName, m_packetHandler};
        if (!gamesHandler.getLogic().isGameFinished()) {
          gamesHandler.getLogic().handleGameLogic(key, request);
        } else {
          std::cout << "Finished\n";
          return false;
        }
      }
    }
  }

  return true;
}

} // namespaces
