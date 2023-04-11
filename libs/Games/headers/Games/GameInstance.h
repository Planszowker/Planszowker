#pragma once

#include <NetworkHandler/SupervisorPacketHandler.h>
#include <ThreadSafeQueue/ThreadSafeQueue.h>
#include <Games/CommObjects.h>

#include <atomic>
#include <thread>
#include <utility>

namespace pla::games {

struct GameInstanceQueueParameters {
  size_t clientId;
  games::Request request;
};

struct GameInstanceSyncParameters {
  utils::ThreadSafeQueue<GameInstanceQueueParameters> queue;
  std::jthread gameServerThread;
};

struct GameInstance {
  GameInstance(network::SupervisorPacketHandler& packetHandler,
               utils::ThreadSafeQueue<GameInstanceQueueParameters>& queue,
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
  utils::ThreadSafeQueue<GameInstanceQueueParameters>& queue;
  std::string gameKey;
  std::size_t creatorId;
  std::vector<size_t> clientsIds;
};

}