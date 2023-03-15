#pragma once

#include <NetworkHandler/SupervisorPacketHandler.h>
#include <ThreadSafeQueue/ThreadSafeQueue.h>
#include <Games/Objects.h>

#include <atomic>
#include <thread>
#include <utility>

namespace pla::games {

struct GameInstanceSyncParameters {
  utils::ThreadSafeQueue<games::Request> queue;
  std::jthread gameServerThread;
};

struct GameInstance {
  GameInstance(network::SupervisorPacketHandler& packetHandler,
               utils::ThreadSafeQueue<games::Request>& queue,
               std::string gameKey,
               std::vector<size_t> clientsIds,
               size_t creatorId)
          : packetHandler(packetHandler)
          , queue(queue)
          , gameKey(std::move(gameKey))
          , clientsIds(std::move(clientsIds))
          , creatorId(creatorId)
  { }
  network::SupervisorPacketHandler& packetHandler;
  utils::ThreadSafeQueue<games::Request>& queue;
  std::string gameKey;
  std::size_t creatorId;
  std::vector<size_t> clientsIds;
};

}