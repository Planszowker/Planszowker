#include <Supervisor/GamesInfoExtractor.h>

#include <PlametaParser/Parser.h>
#include <ErrorHandler/ErrorLogger.h>

#include <filesystem>
#include <regex>
#include <easylogging++.h>

#include <sstream>
#include <fstream>

namespace pla::supervisor {

GamesInfoExtractor::GamesInfoExtractor()
{
  // Iterate over all elements in `scripts/games` directory
  for (const auto& entry : std::filesystem::directory_iterator(GamesDir)) {
    // Check if given directory entry is a `.plagame` file
    std::string filePath = entry.path();
    const std::regex plagameRegex {"\\.plagame"};

    if (std::regex_search(filePath, plagameRegex)) {
      LOG(DEBUG) << "Found PLAGAME: " << filePath;

      // Add entry (relative path) to all available entries
      m_gameEntries.emplace_back(std::move(filePath));
    }
  }

  _getDefaultAssets(); // Get default assets - Thumbnail and board so far
  _getMetaAssets(); // Get meta assets from `.plagame` files - Thumbnail and .plameta file
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
      m_gameMetaAssets.insert({plagameFilePath + "/" + PlametaFile, std::move(plametaStream)});
    } else {
      err_handler::ErrorLogger::printError(".plameta file is mandatory!");
    }

    // Thumbnail does not have to be present
    if (thumbnailStream) {
      LOG(DEBUG) << "   > " << plagameFilePath << " has custom Thumbnail!";
      m_gameMetaAssets.insert({plagameFilePath + "/" + ThumbnailFile, std::move(thumbnailStream)});
    }
  }
}


void GamesInfoExtractor::_getDefaultAssets()
{
  // Iterate over all elements in `scripts/assets` directory
  for (const auto& entry : std::filesystem::directory_iterator(AssetsDir)) {
    std::string filePath = entry.path();

    LOG(DEBUG) << "Found default asset: " << filePath;

    std::shared_ptr<std::ifstream> filePtr = std::make_shared<std::ifstream>(filePath);

    m_gameMetaAssets.insert(std::pair{filePath, std::move(filePtr)});
  }
}

} // Namespace
