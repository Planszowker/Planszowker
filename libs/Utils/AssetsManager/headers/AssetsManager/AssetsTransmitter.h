#pragma once

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
  network::SupervisorPacketHandler& m_packetHandler;
  ZipArchive::Ptr m_plagameFile;
  games_server::GamesHandler::AssetsContainer m_assetsEntries;
};

} // namespace
