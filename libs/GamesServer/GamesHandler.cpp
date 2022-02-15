#include "GamesHandler.h"

namespace pla::common::games::server {

GamesHandler::GamesHandler(const std::string& gameName)
  : m_gameName(gameName)
  , m_plagameFile(GAMES_DIR + m_gameName + GAME_EXTENSION)
{

}

} // namespace
