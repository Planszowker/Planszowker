#include "TextureLoader.h"

namespace pla::common::game_utils {

bool TextureLoader::loadAndStoreTexture(const std::string &filename, const std::string &alias)
{
  std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();
  if (!texture->loadFromFile(filename)) {
    return false;
  }

  auto [insertIt, inserted] = m_textureMap.insert(std::make_pair(alias, std::move(texture)));

  return inserted && (insertIt != m_textureMap.end());
}

std::pair<bool, const std::unique_ptr<sf::Texture>&> TextureLoader::retrieveTexture(const std::string& alias)
{
  auto searchIt = m_textureMap.find(alias);

  static auto dummyReturn = std::make_unique<sf::Texture>();

  if (searchIt != m_textureMap.end()) {
    return {true, searchIt->second};
  }

  // In case we haven't found given texture
  return {false, dummyReturn};
}

} // namespaces
