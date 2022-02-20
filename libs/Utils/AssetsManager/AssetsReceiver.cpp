#include <AssetsReceiver.h>

#include <Games/Objects.h>

#include <AssetsDefines.h>

#include <iostream>
#include <vector>

namespace pla::common::assets {

std::mutex AssetsReceiver::m_assetsMutex;
std::unordered_map<std::string, std::shared_ptr<sf::Texture>> AssetsReceiver::m_assets;
std::vector<std::string> AssetsReceiver::m_assetsNames;

bool AssetsReceiver::parseAndAddAsset(std::deque<sf::Packet>& packets, const std::string& assetName)
{
  const std::scoped_lock assetsMutex(m_assetsMutex); // Obtain mutex

  if (m_assets.find(assetName) != m_assets.end()) {
    std::cerr << "Asset with name " << assetName << " already exists!\n";
    return false;
  }

  std::cout << "Adding asset with name " << assetName << "\n";
  std::shared_ptr<std::vector<char>> recvBuffer = std::make_shared<std::vector<char>>();

  for (auto& packet : packets) {
    // Insert packet's data into buffer
    recvBuffer->insert(recvBuffer->end(),
                       static_cast<const char*>(packet.getData()),
                       static_cast<const char*>(packet.getData()) + packet.getDataSize());
  }

  std::shared_ptr<sf::Texture> _texture = std::make_shared<sf::Texture>();
  _texture->loadFromMemory(recvBuffer->data(), recvBuffer->size());

  m_assets[assetName] = std::move(_texture);
  m_assetsNames.push_back(assetName);
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
