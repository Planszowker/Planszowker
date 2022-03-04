#pragma once

#include <SFML/Network.hpp>
#include <unordered_map>
#include <future>
#include <mutex>
#include <atomic>
#include <functional>
#include <deque>

#include "ClientInfo/ClientInfo.h"
#include "ErrorHandler/ErrorLogger.h"

#include "SupervisorPacketHandler.h"

namespace pla::network {

class ServerPacketHandler : public SupervisorPacketHandler
{
public:
  /*!
  * @brief Constructor with maximum number of players connected at the same time
  */
  explicit ServerPacketHandler(std::atomic_bool& run, size_t maxPlayers);
};

} // namespaces
