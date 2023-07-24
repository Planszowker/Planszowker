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
  m_logic = std::make_unique<Logic>(m_gameInstance.clientsIds, m_gameInstance.gameKey, m_gameInstance.packetHandler, m_gamesHandler.getPlagameFile());

  while(m_run)
  {
    _internalHandling();
  }
}


void ServerHandler::_internalHandling()
{
  if (auto queueParamsOpt = m_gameInstance.queue.pop()){
    std::scoped_lock lock{m_mutex};
    LOG(DEBUG) << "[ServerHandler] Request: " << queueParamsOpt->request.body;

    if (!m_logic->isGameFinished()) {
      m_logic->handleGameLogic(queueParamsOpt->clientId, queueParamsOpt->request);
    } else {
      LOG(DEBUG) << "Finished";
    }
  }
}


void ServerHandler::transmitAssetsToClient(size_t clientId)
{
  std::lock_guard<std::mutex> lock{m_mutex};

  LOG(DEBUG) << "[ServerHandler] Client ID " << clientId;

  // User wants to download game's assets
  auto assetTransmitterPtr = m_assetsTransmitterMap.find(clientId);
  if (assetTransmitterPtr == m_assetsTransmitterMap.end()) {
    // If we haven't found entry with AssetTransmitter, we have to add a new one
    auto [it, inserted] = m_assetsTransmitterMap.insert({clientId, std::make_shared<assets::AssetsTransmitter>(
            m_gamesHandler.getPlagameFile(),
            m_gameInstance.packetHandler,
            m_gamesHandler.getAssetsEntries())});

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
}

} // namespaces
