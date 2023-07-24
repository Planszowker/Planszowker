#include <AssetsReceiver.h>

#include <Games/CommObjects.h>
#include <GamesServer/GamesHandler.h>

#include <easylogging++.h>
#include <base64.hpp>

#include <iostream>
#include <utility>
#include <vector>
#include <regex>

namespace pla::assets {

using namespace games::json_entries;

std::mutex AssetsReceiver::m_assetsMutex;
std::unordered_map<std::string, std::shared_ptr<sf::Texture>> AssetsReceiver::m_assets;
std::vector<std::string> AssetsReceiver::m_assetsNames;
nlohmann::json AssetsReceiver::m_boardDescription;

bool AssetsReceiver::parseAndAddAssets(const nlohmann::json& json)
{
  const std::scoped_lock assetsMutex(m_assetsMutex); // Obtain mutex

  try {
    for (const auto& asset : json) {
      auto assetName = asset.at(ASSET_NAME).get<std::string>();
      auto assetType = asset.at(ASSET_TYPE).get<std::string>();
      auto assetDataB64 = asset.at(ASSET_B64_DATA).get<std::string>();
      std::string assetDataDecoded = base64::decode(assetDataB64);

      if(assetType == "Image") {
        std::regex assetNameRegex {".+/Assets/(.+)"};
        std::smatch matches;
        if (!std::regex_search(assetName, matches, assetNameRegex)) {
          LOG(ERROR) << "Received assets are not in a proper directory!";
          return false;
        }

        auto shortAssetName = matches[1].str();

        LOG(DEBUG) << "[AssetsReceiver] Adding image assets with name " << shortAssetName;
        if (m_assets.find(shortAssetName) != m_assets.end()) {
          LOG(ERROR) << "Asset with name " << shortAssetName << " already exists!";
          return false;
        }

        std::shared_ptr<sf::Texture> _texture = std::make_shared<sf::Texture>();
        _texture->loadFromMemory(assetDataDecoded.c_str(), assetDataDecoded.size());
        _texture->setSmooth(true);

        m_assets.emplace(shortAssetName, std::move(_texture));
        m_assetsNames.push_back(shortAssetName);

      } else if (assetType == "BoardDescription") {
        LOG(DEBUG) << "[AssetsReceiver] Adding board description";

        m_boardDescription = std::move(nlohmann::json::parse(std::move(assetDataDecoded)));
      } else {
        LOG(ERROR) << "[AssetsReceiver] Unknown asset type!";
      }
    }
  } catch(std::exception& e) { }

  return true;
}


std::shared_ptr<sf::Texture> AssetsReceiver::getTexture(std::string name)
{
  const std::scoped_lock assetsMutex(m_assetsMutex); // Obtain mutex

  auto assetPtr = m_assets.find(name);
  if (assetPtr != m_assets.end()) {
    return assetPtr->second;
  }

  // If we haven't found asset with given name
  return nullptr;
}


const std::vector<std::string>& AssetsReceiver::getAssetNames()
{
  const std::scoped_lock assetsMutex(m_assetsMutex); // Obtain mutex

  return m_assetsNames;
}

} // namespace
