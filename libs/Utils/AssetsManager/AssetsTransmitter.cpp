#include <AssetsTransmitter.h>

#include <Games/CommObjects.h>

#include <easylogging++.h>

#include <utility>
#include <nlohmann/json.hpp>

namespace pla::assets {

using namespace games;
using namespace games_server;
using namespace games::json_entries;

AssetsTransmitter::AssetsTransmitter(ZipArchive::Ptr plagameFile, network::SupervisorPacketHandler& packetHandler, GamesHandler::AssetsContainer assetsEntries)
  : m_plagameFile(std::move(plagameFile))
  , m_packetHandler(packetHandler)
  , m_assetsEntries(std::move(assetsEntries))
  , m_currentAssetNameIter(m_assetsEntries.begin())
{
  // Set stream pointer to the first asset available.
  // Remember to close any opened streams (not sure why it is mandatory).
  std::string entryName = m_currentAssetNameIter->first;
  auto entry = m_plagameFile->GetEntry(entryName);

  if (not entry) {
    err_handler::ErrorLogger::printError("[AssetsTransmitter] Entry is nullptr!");
  }

  entry->CloseDecompressionStream();
  m_assetStreamPtr = entry->GetDecompressionStream();
}


void AssetsTransmitter::transmitAssets(size_t clientIdKey)
{
  LOG(DEBUG) << "[AssetsTransmitter] Transmitting assets...";
  if (m_currentAssetNameIter == m_assetsEntries.end()) {
    // We have already sent all the assets.
    LOG(DEBUG) << "[AssetsTransmitter] We have reached end of assets";
    sf::Packet packet;
    Reply reply {
      .type = PacketType::FinishedTransactions,
    };
    packet << reply;
    LOG(DEBUG) << "[AssetsTransmitter] Sending FinishedTransaction for client ID " << clientIdKey;
    m_packetHandler.sendPacketToClient(clientIdKey, packet);
    return;
  }

  // Start asset transaction, if not yet started.
  if (m_transactionCounter == 0) {
    LOG(DEBUG) << "[AssetsTransmitter] Starting transaction";
    _startTransaction(m_currentAssetNameIter->first, m_currentAssetNameIter->second, clientIdKey);
  }

  // We send CHUNK_SIZE packets with data and then wait for response.
  for (size_t iter = 0; iter < CHUNK_QUANTITY; ++iter) {
    m_assetStreamPtr->read(m_buf->data(), CHUNK_SIZE);

    // Increase transaction counter;
    ++m_transactionCounter;

    // Read how many bytes we have successfully read.
    size_t bytesRead = m_assetStreamPtr->gcount();

    // Create packet with chunk data and send it to client.
    sf::Packet packet;
    packet.append(m_buf->data(), bytesRead);
    m_packetHandler.sendPacketToClient(clientIdKey, packet);

    // If we cannot read CHUNK_SIZE bytes, it means EOF.
    if (bytesRead < CHUNK_SIZE) {
      // Move to next asset and clear transaction counter.
      _endTransaction(m_currentAssetNameIter->first, m_currentAssetNameIter->second, clientIdKey);

      ++m_currentAssetNameIter; // Get to next asset
      // Check if we have any more assets...
      if (m_currentAssetNameIter != m_assetsEntries.end()) {
        // Update asset stream
        std::string entryName = m_currentAssetNameIter->first;
        auto entry = m_plagameFile->GetEntry(entryName);

        if(not entry) {
          err_handler::ErrorLogger::printError("[AssetsTransmitter] Entry is nullptr!");
        }

        entry->CloseDecompressionStream();
        m_assetStreamPtr = entry->GetDecompressionStream();
      }
      m_transactionCounter = 0; // Reset transaction counter
      break;
    }
  }
}

void AssetsTransmitter::_startTransaction(const std::string& assetName, const std::string& assetType, size_t key)
{
  sf::Packet packet;
  Reply reply {
    .type = PacketType::StartTransaction,
  };

  nlohmann::json replyJson;
  replyJson[ASSET_NAME] = assetName;
  replyJson[ASSET_TYPE] = assetType;

  reply.body = replyJson.dump();

  packet << reply;

  m_packetHandler.sendPacketToClient(key, packet);
}


void AssetsTransmitter::_endTransaction(const std::string& assetName, const std::string& assetType, size_t key)
{
  sf::Packet packet;
  Reply reply {
    .type = PacketType::EndTransaction,
  };

  nlohmann::json replyJson;
  replyJson[ASSET_NAME] = assetName;
  replyJson[ASSET_TYPE] = assetType;

  reply.body = replyJson.dump();

  packet << reply;

  m_packetHandler.sendPacketToClient(key, packet);
}

} // namespace
