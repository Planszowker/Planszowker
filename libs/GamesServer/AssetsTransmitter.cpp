#include <AssetsTransmitter.h>

#include <Games/Objects.h>
#include <GamesHandler.h>

#include <regex>

namespace pla::common::games::server {

AssetsTransmitter::AssetsTransmitter(zipios::ZipFile& plagameFile, network::ServerPacketHandler& packetHandler, size_t key)
  : m_plagameFile(plagameFile)
  , m_packetHandler(packetHandler)
  , m_key(key)
{
  // Firstly, get list of all files inside assets folder
  _getAssetsList();

  for (const auto& entry : m_assetsEntries) {
    auto buf = std::make_shared<std::vector<char>>(4096);

    zipios::ZipFile::stream_pointer_t entryStream(m_plagameFile.getInputStream(entry->getName()));

    std::cout << "Transferring " << entry->getFileName() << " to client...\n";
    _startTransaction();

    while(true) {
      entryStream->read(buf->data(), PACKET_SIZE);
      size_t bytesRead = entryStream->gcount();

      std::cout << "Read " << bytesRead << " bytes!\n";
      // TODO: Send to client

      if (bytesRead < PACKET_SIZE) {
        break;
      }
    }

    _endTransaction();
  }
}


void AssetsTransmitter::_startTransaction()
{
  sf::Packet packet;
  Reply reply {
    .type = PacketType::DownloadAssets,
    .status = ReplyType::Success,
    .body = "StartTransaction"
  };

  packet << reply;

  m_packetHandler.sendPacketToClient(m_key, packet);
}


void AssetsTransmitter::_transferFile(zipios::ZipFile::stream_pointer_t& fileStream)
{
  // TODO
}


void AssetsTransmitter::_endTransaction()
{
  sf::Packet packet;
  Reply reply {
          .type = PacketType::DownloadAssets,
          .status = ReplyType::Success,
          .body = "EndTransaction"
  };

  packet << reply;

  m_packetHandler.sendPacketToClient(m_key, packet);
}


void AssetsTransmitter::_getAssetsList()
{
  std::cout << "[DEBUG] Trying to get all assets list...\n";
  // Iterate over all entries in .plagame file
  for (const auto& entry : m_plagameFile.entries()) {
    // Find files inside assets folder
    std::regex assetsRegex {GamesHandler::ASSETS_DIR};
    if (std::regex_search(entry->getName(), assetsRegex)) {
      std::cout << "[DEBUG] Found asset " << entry->getFileName() << "!\n";

      m_assetsEntries.push_back(entry);
    }
  }
}

} // namespace
