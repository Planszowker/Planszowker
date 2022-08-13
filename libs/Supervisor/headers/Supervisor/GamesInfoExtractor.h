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
private:
  void _getMetaAssets();

  std::vector<std::string> m_gameEntries;
  std::unordered_map<std::string, zipios::ZipFile::stream_pointer_t> m_gameMetaAssets;
};

}