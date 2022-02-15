#pragma once

#include <zipios/zipfile.hpp>
#include <NetworkHandler/ServerPacketHandler.h>

#include <vector>

namespace pla::common::assets {

class AssetsTransmitter
{
public:
  AssetsTransmitter(zipios::ZipFile& plagameFile, network::ServerPacketHandler& packetHandler, size_t key);

private:
  void _startTransaction(std::string assetName);
  void _transferFile(zipios::ZipFile::stream_pointer_t& fileStream);
  void _endTransaction(std::string assetName);

  void _getAssetsList();

  static constexpr auto PACKET_SIZE {1024};

  network::ServerPacketHandler& m_packetHandler;
  zipios::ZipFile& m_plagameFile;
  size_t m_key;

  std::vector<zipios::FileEntry::pointer_t> m_assetsEntries;
};

} // namespace
