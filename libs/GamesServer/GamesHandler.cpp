#include <GamesHandler.h>

#include <regex>
#include <utility>
#include <easylogging++.h>

namespace pla::games_server {

GamesHandler::GamesHandler(std::string gameName)
  : m_gameName(std::move(gameName))
{
  m_plagameFile = ZipFile::Open(GAMES_DIR + m_gameName + GAME_EXTENSION);

  // We have to get list of all files inside assets folder
  _getAssetsList();
}


void GamesHandler::_getAssetsList()
{
  LOG(DEBUG) << "Trying to get all assets list...";
  // Iterate over all entries in .plagame file
  for (int idx = 0; idx < m_plagameFile->GetEntriesCount(); ++idx) {
    // Find files inside assets folder
    std::regex assetsRegex {std::string(ASSETS_DIR) + "[a-zA-Z0-9]+\\.(jpg|jpeg|png|JPG|JPEG|PNG)"};
    std::string entryName = m_plagameFile->GetEntry(idx)->GetFullName();
    if (std::regex_search(entryName, assetsRegex)) {
      LOG(DEBUG) << "\t> Found asset " << entryName << "!";

      m_assetsEntries.emplace(std::move(entryName), "Image");
      continue;
    }

    // If failed, check if we are dealing with board description
    std::regex boardDescriptionRegex {std::string(BOARD_DESCRIPTION_FILE)};
    if (std::regex_search(entryName, boardDescriptionRegex)) {
      LOG(DEBUG) << "\t> Found board description!";

      m_assetsEntries.emplace(std::move(entryName), "BoardDescription");
      continue;
    }
  }
}

} // namespace
