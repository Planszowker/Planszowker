#include <Supervisor/GamesInfoExtractor.h>

#include <PlametaParser/Parser.h>

#include <filesystem>
#include <regex>
#include <easylogging++.h>

#include <sstream>

namespace pla::supervisor {

GamesInfoExtractor::GamesInfoExtractor()
{
  for (const auto& entry : std::filesystem::directory_iterator(GamesDir)) {
    std::string filePath = entry.path();
    const std::regex plagameRegex {"\\.plagame"};

    if (std::regex_search(filePath, plagameRegex)) {
      LOG(DEBUG) << "Found PLAGAME: " << filePath;

      m_gameEntries.emplace_back(std::move(filePath));
    }
  }

  _getParsers();

  // DEBUG
  for (auto parser : m_parsers) {
    LOG(DEBUG) << (*parser)["overview:description"]->getRaw();
  }
}


void GamesInfoExtractor::_getParsers()
{
  for (const auto& plagameFilePath : m_gameEntries) {
    zipios::ZipFile plagameFile {plagameFilePath};

    zipios::ZipFile::stream_pointer_t is(plagameFile.getInputStream(".plameta"));

    std::stringstream plametaStream;
    plametaStream << is->rdbuf();

    auto parser = std::make_shared<utils::plameta::Parser>(std::move(plametaStream));

    m_parsers.emplace_back(std::move(parser));
  }
}

} // Namespace
