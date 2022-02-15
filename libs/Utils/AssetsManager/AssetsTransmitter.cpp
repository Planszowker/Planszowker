#include <AssetsTransmitter.h>

#include <Games/Objects.h>
#include <GamesServer/GamesHandler.h>

#include <regex>
#include <utility>

namespace pla::common::assets {

using namespace games;

AssetsTransmitter::AssetsTransmitter(zipios::ZipFile& plagameFile, network::ServerPacketHandler& packetHandler, size_t key)
  : m_plagameFile(plagameFile)
  , m_packetHandler(packetHandler)
  , m_key(key)
{
  // Firstly, get list of all files inside assets folder
  _getAssetsList();

  for (const auto& entry : m_assetsEntries) {
    Reply reply {
      .type = games::PacketType::AssetTransaction,
      .status = games::ReplyType::Success
    };

    const size_t packetSize = sizeof(reply) + PACKET_SIZE;
    auto buf = std::make_shared<std::vector<char>>(packetSize);

    zipios::ZipFile::stream_pointer_t entryStream(m_plagameFile.getInputStream(entry->getName()));

    std::cout << "[DEBUG] Transferring " << entry->getFileName() << " to client...\n";
    _startTransaction(entry->getFileName());

    while(true) {
      memcpy(buf->data(), reinterpret_cast<void*>(&reply), sizeof(reply));
      entryStream->read(buf->data() + sizeof(reply), PACKET_SIZE);
      size_t bytesRead = entryStream->gcount();

      std::cout << "[DEBUG] Read " << sizeof(reply) + bytesRead << " bytes!\n";
      m_packetHandler.sendToClient(key, buf->data(), sizeof(reply) + bytesRead);

      if (bytesRead < PACKET_SIZE) {
        break;
      }
    }

    _endTransaction(entry->getFileName());
  }
}


void AssetsTransmitter::_startTransaction(std::string assetName)
{
  sf::Packet packet;
  Reply reply {
    .type = PacketType::StartTransaction,
    .status = ReplyType::Success,
    .body = std::move(assetName)
  };

  packet << reply;

  m_packetHandler.sendPacketToClient(m_key, packet);
}


void AssetsTransmitter::_transferFile(zipios::ZipFile::stream_pointer_t& fileStream)
{
  // TODO
}


void AssetsTransmitter::_endTransaction(std::string assetName)
{
  sf::Packet packet;
  Reply reply {
          .type = PacketType::EndTransaction,
          .status = ReplyType::Success,
          .body = std::move(assetName)
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
    std::regex assetsRegex {games::server::GamesHandler::ASSETS_DIR};
    if (std::regex_search(entry->getName(), assetsRegex)) {
      std::cout << "[DEBUG] Found asset " << entry->getFileName() << "!\n";

      m_assetsEntries.push_back(entry);
    }
  }
}

} // namespace
