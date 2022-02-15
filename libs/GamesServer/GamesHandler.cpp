#include "GamesHandler.h"

namespace pla::common::games::server {

GamesHandler::GamesHandler(std::vector<size_t> &clientIds, const std::string &gameName,
                           network::ServerPacketHandler &packetHandler)
  : m_logic(clientIds, gameName, packetHandler, m_plaGameFile)
  , m_networkHandler(packetHandler)
  , m_gameName(gameName)
  , m_plaGameFile(GAMES_DIR + m_gameName + GAME_EXTENSION)
{

}

} // namespace
