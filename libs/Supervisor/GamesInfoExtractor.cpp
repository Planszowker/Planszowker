#include <Supervisor/GamesInfoExtractor.h>

#include <PlametaParser/Parser.h>
#include <ErrorHandler/ErrorLogger.h>

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

  _getMetaAssets();
}


void GamesInfoExtractor::_getMetaAssets()
{
  // Look for .plameta and Thumbnail in found games
  // Entry looks like this: `scripts/games/GameName.plagame`
  for (const auto& plagameFilePath : m_gameEntries) {
    zipios::ZipFile plagameFile {plagameFilePath};

    zipios::ZipFile::stream_pointer_t plametaStream(plagameFile.getInputStream(PlametaFile));
    zipios::ZipFile::stream_pointer_t thumbnailStream(plagameFile.getInputStream(ThumbnailFile));

    // Add .plameta and thumbnail streams (if exist) to the meta assets map
    if (plametaStream) {
      m_gameMetaAssets.insert({plagameFilePath + "/" + PlametaFile, plametaStream});
    } else {
      err_handler::ErrorLogger::printError(".plameta file is mandatory!");
    }

    // Thumbnail does not have to be present
    if (thumbnailStream) {
      m_gameMetaAssets.insert({plagameFilePath + "/" + ThumbnailFile, thumbnailStream});
    }
  }
}

} // Namespace
