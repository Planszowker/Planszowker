#pragma once

#include <vector>
#include <string>

#include <zipios/zipfile.hpp>

#include <NetworkHandler/ServerPacketHandler.h>

namespace pla::common::games::server {

class GamesHandler {
public:
  explicit GamesHandler(const std::string& gameName);

  zipios::ZipFile& getPlagameFile() { return m_plagameFile; }

  static constexpr auto GAME_EXTENSION = ".plagame";
  static constexpr auto BOARD_DESCRIPTION_FILE = "BoardDescription.json";
  static constexpr auto GAMES_DIR = "scripts/games/";
  static constexpr auto DEFAULT_ASSETS_DIR = "assets";
  static constexpr auto ASSETS_DIR = "Assets/";
  static constexpr auto LUA_SCRIPT_EXTENSION = ".lua";
  static constexpr auto LUA_SCRIPT_INIT_SUFFIX = "-init.lua";
private:
  const std::string& m_gameName;

  zipios::ZipFile m_plagameFile;
};

} // namespace
