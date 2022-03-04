#include "ServerPacketHandler.h"

#include "Logger/Logger.h"
#include "Games/Objects.h"
#include "TimeMeasurement/TimeLogger.h"
#include "CompilerUtils/FunctionInfoExtractor.h"

using namespace pla::logger;
using namespace pla::time_measurement;

namespace pla::network {

ServerPacketHandler::ServerPacketHandler(std::atomic_bool& run, size_t maxPlayers)
  : SupervisorPacketHandler(run)
{
  m_maxPlayers = maxPlayers;
}

} // namespaces
