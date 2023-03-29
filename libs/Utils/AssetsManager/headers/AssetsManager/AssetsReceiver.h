#pragma once

#include <NetworkHandler/ClientPacketHandler.h>

#include <unordered_map>
#include <deque>
#include <mutex>

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include <nlohmann/json.hpp>

namespace pla::assets {

class AssetsReceiver
{
public:
  static bool parseAndAddAssets(const nlohmann::json& json);

  static std::shared_ptr<sf::Texture> getTexture(std::string name);

  static const std::vector<std::string>& getAssetNames();

  static const nlohmann::json& getBoardDescription() { return m_boardDescription; }

private:
  static std::mutex m_assetsMutex;

  static std::vector<std::string> m_assetsNames;

  static nlohmann::json m_boardDescription;

  static std::unordered_map<std::string, std::shared_ptr<sf::Texture>> m_assets;
};

} // namespace
