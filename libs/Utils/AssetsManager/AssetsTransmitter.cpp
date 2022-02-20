#include <AssetsTransmitter.h>

#include <Games/Objects.h>
#include <GamesServer/GamesHandler.h>

#include <utility>

namespace pla::common::assets {

using namespace games;

// TODO: Remove debug printout ;)
AssetsTransmitter::AssetsTransmitter(zipios::ZipFile& plagameFile, network::ServerPacketHandler& packetHandler, std::vector<std::string>& assetsEntries)
  : m_plagameFile(plagameFile)
  , m_packetHandler(packetHandler)
  , m_assetsEntries(assetsEntries)
  , m_currentAssetNamePtr(assetsEntries.begin())
{
  // Set stream pointer to the first asset available.
  m_assetStreamPtr = m_plagameFile.getInputStream(*(m_currentAssetNamePtr));
}


void AssetsTransmitter::transmitAssets(size_t key)
{
  //std::cout << "[DEBUG] Transmitting assets...\n";
  if (m_currentAssetNamePtr == m_assetsEntries.end()) {
    // We have already sent all the assets.
    std::cout << "[DEBUG] We have reached end of assets...\n";
    return;
  }

  // Start asset transaction, if not yet started.
  if (m_transactionCounter == 0) {
    std::cout << "[DEBUG] Starting transaction...\n";
    _startTransaction(*m_currentAssetNamePtr, key);
  }

  // We send CHUNK_SIZE packets with data and then wait for response.
  for (size_t iter = 0; iter < CHUNK_QUANTITY; ++iter) {
    //std::cout << "[DEBUG] Transferring " << m_transactionCounter << " chunk to client...\n";
    m_assetStreamPtr->read(m_buf->data(), CHUNK_SIZE);

    // Increase transaction counter;
    ++m_transactionCounter;

    // Read how many bytes we have successfully read.
    size_t bytesRead = m_assetStreamPtr->gcount();

    // Create packet with chunk data and send it to client.
    sf::Packet packet;
    packet.append(m_buf->data(), bytesRead);
    m_packetHandler.sendPacketToClient(key, packet);

    //std::cout << "[DEBUG] Bytes read: " << bytesRead << "\n";

    // If we cannot read CHUNK_SIZE bytes, it means EOF.
    if (bytesRead < CHUNK_SIZE) {
      // Move to next asset and clear transaction counter.
      _endTransaction(*m_currentAssetNamePtr, key);

      ++m_currentAssetNamePtr; // Get to next asset
      // Check if we have any more assets...
      if (m_currentAssetNamePtr != m_assetsEntries.end()) {
        // Update asset stream
        m_assetStreamPtr = m_plagameFile.getInputStream(*(m_currentAssetNamePtr));
      }
      m_transactionCounter = 0; // Reset transaction counter
      break;
    }
  }
}


void AssetsTransmitter::_startTransaction(std::string assetName, size_t key)
{
  sf::Packet packet;
  Reply reply {
    .type = PacketType::StartTransaction,
    .status = ReplyType::Success,
    .body = std::move(assetName)
  };

  packet << reply;

  m_packetHandler.sendPacketToClient(key, packet);
}


void AssetsTransmitter::_transferFile(zipios::ZipFile::stream_pointer_t& fileStream)
{
  // TODO
}


void AssetsTransmitter::_endTransaction(std::string assetName, size_t key)
{
  sf::Packet packet;
  Reply reply {
          .type = PacketType::EndTransaction,
          .status = ReplyType::Success,
          .body = std::move(assetName)
  };

  packet << reply;

  m_packetHandler.sendPacketToClient(key, packet);
}

} // namespace
