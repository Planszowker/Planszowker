#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

namespace pla::common::game_utils {

class TextureLoader
{
public:
  bool loadAndStoreTexture(const std::string& filename, const std::string& alias);
  std::pair<bool, const std::unique_ptr<sf::Texture>&> retrieveTexture(const std::string& alias);
private:
  std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_textureMap;
};

} // namespaces
