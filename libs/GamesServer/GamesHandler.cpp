#include <GamesHandler.h>

#include <regex>
#include <easylogging++.h>

namespace pla::games_server {

GamesHandler::GamesHandler(const std::string& gameName)
  : m_gameName(gameName)
{
  m_plagameFile = ZipFile::Open(GAMES_DIR + m_gameName + GAME_EXTENSION);

  // We have to get list of all files inside assets folder
  _getAssetsList();
}


void GamesHandler::_getAssetsList()
{
  LOG(DEBUG) << "Trying to get all assets list...\n";
  // Iterate over all entries in .plagame file
  for (int idx = 0; idx < m_plagameFile->GetEntriesCount(); ++idx) {
    // Find files inside assets folder
    std::regex assetsRegex {ASSETS_DIR};
    std::string entryName = m_plagameFile->GetEntry(idx)->GetFullName();
    if (std::regex_search(entryName, assetsRegex)) {
      LOG(DEBUG) << "Found asset " << entryName << "!\n";

      m_assetsEntries.push_back(entryName);
    }
  }
}

} // namespace
