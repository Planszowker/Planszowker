#include <ServerHandler.h>

#include <TimeMeasurement/TimeLogger.h>
#include <CompilerUtils/FunctionInfoExtractor.h>
#include <Games/CommObjects.h>
#include <Logic.h>
#include <AssetsManager/AssetsTransmitter.h>

#include <easylogging++.h>

namespace pla::games_server {

using namespace games;

void ServerHandler::run()
{
  while(m_run)
  {
    if (!_internalHandling()) {
      break;
    }
  }
}


bool ServerHandler::_internalHandling() {
  // TODO: Time logger probably not needed, since current thread is already suspended while waiting for new data in queue
  // time_measurement::TimeLogger timeLogger(GET_CURRENT_FUNCTION_NAME());

//  std::vector<size_t> keys;
//  auto packetsMap = m_packetHandler.getPackets(keys);
//
//  // TODO: Not elegant, maybe better solution?
//  for (const auto& key: keys) {
//    // Find map entry with client ID -> (size_t, deque) is received
//    const auto mapIt = packetsMap.find(key);
//    if (mapIt == packetsMap.end()) {
//      continue;
//    }
//
//    // Iterate over all packets in deque
//    for (auto& packet: mapIt->second) {
//      Request request{};
//      packet >> request;
//
//      if (request.type == PacketType::Heartbeat) {
//        // We don't care about Heartbeat packets
//        continue;
//      } else if (request.type == PacketType::ID) {
//        // If we get ID request, we need to send client's ID
//        Reply idReply{
//          .type = PacketType::ID,
//          .status = ReplyType::Success,
//          .body = std::to_string(key)
//        };
//        sf::Packet replyPacket;
//        replyPacket << idReply;
//
//        m_packetHandler.sendPacketToClient(key, replyPacket);
//        continue;
//      } else if (request.type == PacketType::DownloadAssets) {
//        // User wants to download game's assets
//        auto assetTransmitterPtr = m_assetsTransmitterMap.find(key);
//        if (assetTransmitterPtr == m_assetsTransmitterMap.end()) {
//          // If we haven't found entry with AssetTransmitter, we have to add a new one
//          auto [it, inserted] = m_assetsTransmitterMap.insert({key, std::make_shared<assets::AssetsTransmitter>(
//                  m_gamesHandler.getPlagameFile(),
//                  m_packetHandler,
//                  m_gamesHandler.getAssetsEntries())});
//
//          if (inserted) {
//            assetTransmitterPtr = it;
//          } else {
//            std::cerr << "Cannot add new asset transmitter to map!\n";
//          }
//        }
//
//        // Transmit assets in chunks
//        assetTransmitterPtr->second->transmitAssets(key);
//
//        continue;
//      }
//
//      std::cout << "Type: " << static_cast<int>(request.type) << "\n";
//      std::cout << "Body: " << request.body << "\n";
//
//      if (m_packetHandler.hasEnoughClientsConnected()) {
//        static Logic logic {keys, m_gameName, m_packetHandler, m_gamesHandler.getPlagameFile()};
//        if (!logic.isGameFinished()) {
//          logic.handleGameLogic(key, request);
//        } else {
//          std::cout << "Finished\n";
//          return false;
//        }
//      }
//    }
//  }

  LOG(DEBUG) << "[ServerHandler] Waiting for queue";
  games::Request request = m_gameInstance.queue.pop();
  std::lock_guard<std::mutex> lock{m_mutex};
  LOG(DEBUG) << "[ServerHandler] Request: " << (int)request.type;

  return true;
}


void ServerHandler::transmitAssetsToClient(size_t clientId)
{
  std::lock_guard<std::mutex> lock{m_mutex};

  // User wants to download game's assets
  auto assetTransmitterPtr = m_assetsTransmitterMap.find(clientId);
  if (assetTransmitterPtr == m_assetsTransmitterMap.end()) {
    // If we haven't found entry with AssetTransmitter, we have to add a new one
    auto [it, inserted] = m_assetsTransmitterMap.insert({clientId, std::make_shared<assets::AssetsTransmitter>(
            std::move(m_gamesHandler.getPlagameFile()),
            m_gameInstance.packetHandler,
            std::move(m_gamesHandler.getAssetsEntries()))});

    if (inserted) {
      assetTransmitterPtr = it;
    } else {
      err_handler::ErrorLogger::printError("Cannot add new asset transmitter to map!");
    }
  }

  // Transmit assets
  assetTransmitterPtr->second->transmitAssets(clientId);
}


void ServerHandler::stop()
{
  m_run = false;

  // Server handler thread is waiting for an item in queue to appear.
  // We send an empty request to end this thread.
  m_gameInstance.queue.push(games::Request{});
}

} // namespaces
