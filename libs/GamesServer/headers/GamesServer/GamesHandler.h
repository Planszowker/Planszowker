#pragma once

#include <vector>
#include <string>

#include <zipios/zipfile.hpp>

#include <NetworkHandler/ServerPacketHandler.h>

#include <Logic.h>

namespace pla::common::games::server {

class GamesHandler {
public:
  GamesHandler(std::vector<size_t> &clientIds, const std::string &gameName,
               network::ServerPacketHandler &packetHandler);

  Logic& getLogic() { return m_logic; }

  static constexpr auto GAME_EXTENSION = ".plagame";
  static constexpr auto BOARD_DESCRIPTION_FILE = "BoardDescription.json";
  static constexpr auto GAMES_DIR = "scripts/games/";
  static constexpr auto ASSETS_DIR = "assets";
  static constexpr auto LUA_SCRIPT_EXTENSION = ".lua";
  static constexpr auto LUA_SCRIPT_INIT_SUFFIX = "-init.lua";
private:
  const std::string& m_gameName;

  zipios::ZipFile m_plaGameFile;

  Logic m_logic;

  network::ServerPacketHandler& m_networkHandler;
};

} // namespace
