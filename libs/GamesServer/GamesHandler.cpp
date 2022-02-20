#include "GamesHandler.h"

#include <regex>

namespace pla::common::games::server {

GamesHandler::GamesHandler(const std::string& gameName)
  : m_gameName(gameName)
  , m_plagameFile(GAMES_DIR + m_gameName + GAME_EXTENSION)
{
  // We have to get list of all files inside assets folder
  _getAssetsList();
}


void GamesHandler::_getAssetsList()
{
  std::cout << "[DEBUG] Trying to get all assets list...\n";
  // Iterate over all entries in .plagame file
  for (const auto& entry : m_plagameFile.entries()) {
    // Find files inside assets folder
    std::regex assetsRegex {ASSETS_DIR};
    if (std::regex_search(entry->getName(), assetsRegex)) {
      std::cout << "[DEBUG] Found asset " << entry->getFileName() << "!\n";

      m_assetsEntries.push_back(entry->getName());
    }
  }
}

} // namespace
