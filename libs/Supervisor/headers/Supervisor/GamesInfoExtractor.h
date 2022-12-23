#pragma once

#include <zipios/zipfile.hpp>

#include <PlametaParser/Parser.h>

#include <vector>
#include <sstream>

namespace pla::supervisor {

class GamesInfoExtractor
{
public:
  GamesInfoExtractor();

  static constexpr auto AssetsDir = "scripts/assets";
  static constexpr auto GamesDir = "scripts/games";
  static constexpr auto PlametaFile = ".plameta";
  static constexpr auto ThumbnailFile = "Thumbnail.png";
  static constexpr auto CombinedStringDelimiter = "::";

  static constexpr auto DefaultBoard = "default-board.jpg";
  static constexpr auto DefaultThumbnail = "default-thumbnail.png";

  using GameEntriesContainer = std::vector<std::string>;
  using GameMetaAssetsContainer = std::unordered_map<std::string, std::shared_ptr<std::istream>>;

  const GameEntriesContainer& getEntries()
  {
    return m_gameEntries;
  }

  const GameMetaAssetsContainer& getMetaAssets()
  {
    return m_gameMetaAssets;
  }

private:
  void _getMetaAssets();
  void _getDefaultAssets();

  GameEntriesContainer m_gameEntries;
  GameMetaAssetsContainer m_gameMetaAssets;
};

}