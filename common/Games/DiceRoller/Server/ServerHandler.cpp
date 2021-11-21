
#include <Games/ServerHandler.h>

#include "ServerHandler.h"

#include "DiceRoller/Objects.h"
#include "ErrorHandler/ErrorLogger.h"
#include "Logic.h"

// Debug info
#include "CompilerUtils/FunctionInfoExtractor.h"
#include "TimeMeasurement/TimeLogger.h"

namespace pla::common::games::dice_roller {

using namespace err_handler;

DiceRollerServerHandler::DiceRollerServerHandler(network::ServerPacketHandler& packetHandler)
  : ServerHandler(packetHandler)
{
}


bool DiceRollerServerHandler::_internalHandling() {
  if (!m_packetHandler.hasEnoughClientsConnected()) {
    return true;
  }

  time_measurement::TimeLogger timeLogger(GET_CURRENT_FUNCTION_NAME());

  std::vector<size_t> keys;
  auto packetsMap = m_packetHandler.getPackets(keys);

  // TODO: Not elegant, maybe better solution?
  static DiceRollerLogic logic{keys};

  for (const auto& key: keys) {
    // Find map entry with client ID -> (size_t, deque) is received
    const auto mapIt = packetsMap.find(key);
    if (mapIt == packetsMap.end()) {
      continue;
    }

    // Iterate over all packets in deque
    for (const auto& packet: mapIt->second) {
      DiceRollerRequest request{};
      if (packet.getDataSize() < sizeof(request)) {
        // Wrong packet
        continue;
      }

      request = *reinterpret_cast<DiceRollerRequest*>(const_cast<void*>(packet.getData()));

      if (request.type == DiceRollerRequestType::GetMyId) {
        sf::Packet getMyIdReply;
        DiceRollerReply reply { .replyType = DiceRollerReplyType::GetIdReply };
        DiceRollerGetIdData getIdData { .clientId = key };


        getMyIdReply.append(&reply, sizeof(reply));
        getMyIdReply.append(&getIdData, sizeof(getIdData));

        m_packetHandler.sendPacketToClient(key, getMyIdReply);
      }

      logic.handleGameLogic(key, request.type, m_packetHandler);
    }
  }

  return true;
}

}

// namespaces
