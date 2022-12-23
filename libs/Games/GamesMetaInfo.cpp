#include "GamesMetaInfo.h"

#include <Supervisor/GamesInfoExtractor.h>

#include <easylogging++.h>
#include <cstring>
#include <regex>

namespace pla::games {

using namespace supervisor;

void GamesMetaInfo::addMetaData(const std::string& combinedString)
{
  // Look for special character in received combined string
  size_t pos = combinedString.find(GamesInfoExtractor::CombinedStringDelimiter);
  if (pos != std::string::npos) {
    std::string filename = combinedString.substr(0, pos);
    std::string content = combinedString.substr(pos + std::strlen(GamesInfoExtractor::CombinedStringDelimiter));

    const std::regex plagameNameRegex {"([a-zA-Z0-9]+)(\\.plagame)"};
    const std::regex defaultAssetNameRegex {"(DefaultThumbnail)(\\.[a-zA-Z0-9]{1,6})"};
    std::smatch matches;

    // We have to make sure we deal with `.plagame` file
    if (std::regex_search(filename, matches, plagameNameRegex)) {

      // Extract game's name from file path - although spaces are permitted, they should not be used
      std::string key = matches[1].str();
      LOG(DEBUG) << "Found game name: " << key;

      // Create REGEX to look for .plameta and Thumbnail.png entries
      constexpr auto commonRegexStr {"[a-zA-Z0-9/]+"};

      std::stringstream plametaRegexSs;
      plametaRegexSs << commonRegexStr << GamesInfoExtractor::PlametaFile;
      const std::regex plametaRegex {plametaRegexSs.str()};

      std::stringstream thumbnailRegexSs;
      thumbnailRegexSs << commonRegexStr << GamesInfoExtractor::ThumbnailFile;
      const std::regex thumbnailRegex {thumbnailRegexSs.str()};

      if (std::regex_search(filename, matches, plametaRegex)) {
        // There's a .plameta file to handle
        LOG(DEBUG) << "\t>>> .plameta to handle!";

        std::stringstream ss;
        ss << content;
        utils::plameta::Parser parser {std::move(ss)};
        m_plametas.insert(std::pair(key, parser));
      } else if (std::regex_search(filename, matches, thumbnailRegex)) {
        // There's a Thumbnail.png file to handle
        LOG(DEBUG) << "\t>>> Thumbnail.png to handle!";

        std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
        texture->loadFromMemory(content.c_str(), content.size());

        m_thumbnails.insert(std::pair(key, std::move(texture)));
      }
    }
    // Otherwise we should check if we want to handle default assets
    else if (std::regex_search(filename, matches, defaultAssetNameRegex)) {
      // Extract game's name from file path - although spaces are permitted, they should not be used
      std::string key = matches[1].str();
      LOG(DEBUG) << "Found default asset name: " << key;

      std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
      texture->loadFromMemory(content.c_str(), content.size());

      m_thumbnails.insert(std::pair(key, std::move(texture)));
    }
  }

  /*
  for (const auto& ziu: m_thumbnails) {
    LOG(DEBUG) << "\t >>> " << ziu.first;
  }
  */
}

void GamesMetaInfo::clear()
{
  m_thumbnails.clear();
  m_plametas.clear();
}

} // namespace
