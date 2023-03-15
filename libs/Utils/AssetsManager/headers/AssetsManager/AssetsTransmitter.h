#pragma once

#include <ZipLib/ZipFile.h>
#include <NetworkHandler/SupervisorPacketHandler.h>

#include "AssetsDefines.h"

#include <vector>
#include <istream>

namespace pla::assets {

class AssetsTransmitter
{
public:
  AssetsTransmitter(ZipArchive::Ptr plagameFile, network::SupervisorPacketHandler& packetHandler, std::vector<std::string>& assetsEntries);

  // Transmit all available assets in chunks - to not overflow eth fifo on client's side
  void transmitAssets(size_t clientKey);

  // Transmit specific asset in chunks
  bool transmitAsset(size_t clientKey, const std::string& assetName);

private:
  void _startTransaction(std::string assetName, size_t key);
  void _transferFile(ZipArchiveEntry::Ptr fileStream);
  void _endTransaction(std::string assetName, size_t key);

  network::SupervisorPacketHandler& m_packetHandler;
  ZipArchive::Ptr m_plagameFile;
  std::vector<std::string>& m_assetsEntries;
  std::vector<std::string>::iterator m_currentAssetNameIter;

  // Chunk buffer
  std::shared_ptr<std::vector<char>> m_buf = std::make_shared<std::vector<char>>(CHUNK_SIZE);

  // Client specific containers
  size_t m_transactionCounter {0};
  std::istream* m_assetStreamPtr;
};

} // namespace
