#pragma once

#include <AssetsManager/AssetsDefines.h>

#include <ZipLib/ZipFile.h>
#include <NetworkHandler/SupervisorPacketHandler.h>
#include <GamesServer/GamesHandler.h>

#include <vector>
#include <istream>

namespace pla::assets {

class AssetsTransmitter
{
public:
  AssetsTransmitter(ZipArchive::Ptr plagameFile, network::SupervisorPacketHandler& packetHandler, games_server::GamesHandler::AssetsContainer assetsEntries);

  void transmitAssets(size_t clientIdKey); // Transmit all available assets in chunks

private:
  void _startTransaction(const std::string& assetName, const std::string& assetType, size_t key);
  void _endTransaction(const std::string& assetName, const std::string& assetType, size_t key);

  network::SupervisorPacketHandler& m_packetHandler;
  ZipArchive::Ptr m_plagameFile;
  games_server::GamesHandler::AssetsContainer m_assetsEntries;
  games_server::GamesHandler::AssetsContainer::iterator m_currentAssetNameIter;

  // Chunk buffer
  std::shared_ptr<std::vector<char>> m_buf = std::make_shared<std::vector<char>>(CHUNK_SIZE);

  // Client specific containers
  size_t m_transactionCounter {0};
  std::istream* m_assetStreamPtr;
};

} // namespace
