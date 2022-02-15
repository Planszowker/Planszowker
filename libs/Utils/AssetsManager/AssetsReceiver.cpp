#include <AssetsReceiver.h>

#include <Games/Objects.h>

#include <iostream>
#include <vector>

namespace pla::common::assets {

std::mutex AssetsReceiver::m_assetsMutex;
std::unordered_map<std::string, std::shared_ptr<sf::Texture>> AssetsReceiver::m_assets;

bool AssetsReceiver::parseAndAddAsset(std::deque<sf::Packet>& packets, const std::string& assetName)
{
  const std::scoped_lock assetsMutex(m_assetsMutex); // Obtain mutex

  if (m_assets.find(assetName) != m_assets.end()) {
    std::cerr << "Asset with name " << assetName << " already exists!\n";
    return false;
  }

  std::cout << "Adding asset with name " << assetName << "\n";
  std::shared_ptr<std::vector<char>> assetBuffer = std::make_shared<std::vector<char>>();

  // TODO: Possible problem when client is on platform that has different reply's size that server has...
  games::Reply reply;
  for (auto& packet : packets) {
    // Insert packet's data into buffer
    assetBuffer->insert(assetBuffer->end(), static_cast<const char*>(packet.getData()), static_cast<const char*>(packet.getData()) + packet.getDataSize());
  }

  m_assets[assetName] = std::move(std::reinterpret_pointer_cast<sf::Texture>(assetBuffer));
  return true;
}


static std::shared_ptr<sf::Texture*> getTexture(std::string name)
{
  return nullptr;
}

} // namespace
