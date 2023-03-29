#include <AssetsTransmitter.h>

#include <Games/CommObjects.h>

#include <easylogging++.h>

#include <utility>
#include <nlohmann/json.hpp>
#include <base64.hpp>

namespace pla::assets {

using namespace games;
using namespace games_server;
using namespace games::json_entries;

AssetsTransmitter::AssetsTransmitter(ZipArchive::Ptr plagameFile, network::SupervisorPacketHandler& packetHandler, GamesHandler::AssetsContainer assetsEntries)
  : m_plagameFile(std::move(plagameFile))
  , m_packetHandler(packetHandler)
  , m_assetsEntries(std::move(assetsEntries))
{
}


void AssetsTransmitter::transmitAssets(size_t clientIdKey)
{
  LOG(DEBUG) << "[AssetsTransmitter] Transmitting assets...";
  sf::Packet packet;
  nlohmann::json replyJson = nlohmann::json::array();

  for (const auto& [assetName, assetType] : m_assetsEntries) {

    auto entry = m_plagameFile->GetEntry(assetName);

    if (not entry) {
      err_handler::ErrorLogger::printError("[AssetsTransmitter] Entry is nullptr!");
    }

    auto assetStream = entry->GetDecompressionStream();
    assetStream->unsetf(std::ios_base::skipws);

    // Convert asset to Base64 string
    std::string assetStr {std::istream_iterator<char>{*assetStream}, {}};
    std::string base64Asset = base64::encode(assetStr);


    nlohmann::json assetJson;
    assetJson[ASSET_NAME] = assetName;
    assetJson[ASSET_TYPE] = assetType;
    assetJson[ASSET_B64_DATA] = base64Asset;

    replyJson.push_back(assetJson);

    entry->CloseDecompressionStream();
  }

  games::Reply reply {
    .type = games::PacketType::DownloadAssets,
    .body = replyJson.dump(),
  };
  packet << reply;
  m_packetHandler.sendPacketToClient(clientIdKey, packet);
}

} // namespace
