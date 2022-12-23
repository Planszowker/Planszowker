#pragma once

#include <PlametaParser/Parser.h>

#include <SFML/Graphics.hpp>

#include <unordered_map>

namespace pla::games {

/*!
 * @brief Class to hold .plameta and Thumbnails for all available games
 */
class GamesMetaInfo {
public:
  using PlametaContainer = std::unordered_map<std::string, utils::plameta::Parser>;
  using ThumbnailContainer = std::unordered_map<std::string, std::shared_ptr<sf::Texture>>;

  void addMetaData(const std::string& combinedString);
  void clear();

  [[nodiscard]] [[maybe_unused]]
  const PlametaContainer& getPlametaParsers() const
  {
    return this->m_plametas;
  }

  [[nodiscard]] [[maybe_unused]]
  const ThumbnailContainer& getThumbnails() const
  {
    return this->m_thumbnails;
  }

private:
  // String key - game's name (e.g. DiceRoller)
  // Extracted from .plagame path
  PlametaContainer m_plametas;
  ThumbnailContainer m_thumbnails;
};

} // namespace
