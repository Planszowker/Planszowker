#pragma once

#include <NetworkHandler/ClientPacketHandler.h>

#include <unordered_map>
#include <deque>
#include <mutex>

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

namespace pla::assets {

class AssetsReceiver
{
public:
  static bool parseAndAddAsset(std::deque<sf::Packet>& packets, const std::string& assetName);

  static std::shared_ptr<sf::Texture> getTexture(std::string name);

  static const std::vector<std::string>& getAssetNames();

  static bool addBoardDescription(std::deque<sf::Packet>& packets, const std::string& boardDescription);
  static std::string getBoardDescription() { return m_boardDescription; }

private:
  static std::mutex m_assetsMutex;

  static std::vector<std::string> m_assetsNames;

  static std::string m_boardDescription;

  static std::unordered_map<std::string, std::shared_ptr<sf::Texture>> m_assets;
};

} // namespace
