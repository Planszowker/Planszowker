#pragma once

#include <zipios/zipfile.hpp>

#include <PlametaParser/Parser.h>

#include <vector>

namespace pla::supervisor {

class GamesInfoExtractor
{
public:
  GamesInfoExtractor();

  static constexpr auto GamesDir = "scripts/games";
  static constexpr auto PlametaFile = ".plameta";
  static constexpr auto ThumbnailFile = "Thumbnail.png";

  using GameEntriesContainer = std::vector<std::string>;
  using GameMetaAssetsContainer = std::unordered_map<std::string, zipios::ZipFile::stream_pointer_t>;

  GameEntriesContainer& getEntries()
  {
    return m_gameEntries;
  }

  GameMetaAssetsContainer& getMetaAssets()
  {
    return m_gameMetaAssets;
  }


private:
  void _getMetaAssets();

  GameEntriesContainer m_gameEntries;
  GameMetaAssetsContainer m_gameMetaAssets;
};

}