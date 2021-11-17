
#include <Games/ServerHandler.h>

#include "ServerHandler.h"

#include "DiceRoller/Objects.h"
#include "ErrorHandler/ErrorLogger.h"
#include "Logic.h"

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
    for (auto packet: mapIt->second) {
      DiceRollerRequest request;
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

    m_packetHandler.clearPacketsForClient(key);
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  return true;
}

}

// namespaces
