#include "ServerHandler.h"
#include "TimeMeasurement/TimeLogger.h"
#include "CompilerUtils/FunctionInfoExtractor.h"
#include "Games/Objects.h"
#include "Logic.h"

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
}


bool ServerHandler::_internalHandling() {
  if (!m_packetHandler.hasEnoughClientsConnected()) {
    return true;
  }

  time_measurement::TimeLogger timeLogger(GET_CURRENT_FUNCTION_NAME());

  std::vector<size_t> keys;
  auto packetsMap = m_packetHandler.getPackets(keys);

  if (!keys.empty()) {
    //std::cout << "keys.size() = " << keys.size() << "\n";
  }

  // TODO: Not elegant, maybe better solution?
  for (const auto& key: keys) {
    // Find map entry with client ID -> (size_t, deque) is received
    const auto mapIt = packetsMap.find(key);
    if (mapIt == packetsMap.end()) {
      continue;
    }

    std::cout << "DEBUG" << std::endl;
    // Iterate over all packets in deque
    for (const auto& packet: mapIt->second) {
      Request request{};
      if (packet.getDataSize() < sizeof(request)) {
        // Wrong packet
        continue;
      }

      request = *reinterpret_cast<Request *>(const_cast<void *>(packet.getData()));
      if (m_packetHandler.hasEnoughClientsConnected()) {
        static Logic logic{keys, m_gameName};
        if (!logic.isGameFinished()) {
          logic.handleGameLogic(key, request, m_packetHandler);
        }
      }
    }
  }

  return true;
}

} // namespaces
