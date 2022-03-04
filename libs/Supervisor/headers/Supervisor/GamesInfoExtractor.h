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
private:
  void _getParsers();

  std::vector<std::string> m_gameEntries;
  std::vector<zipios::FileEntry::pointer_t> m_plametaFilesStreams;

  std::vector<std::shared_ptr<utils::plameta::Parser>> m_parsers;
};

}