#pragma once

#include <vector>
#include <string>
#include <filesystem>

#include <ZipLib/ZipFile.h>

#include <NetworkHandler/ServerPacketHandler.h>

namespace pla::games_server {

class GamesHandler {
public:
  explicit GamesHandler(const std::string& gameName);

  ZipArchive::Ptr getPlagameFile() { return m_plagameFile; }

  std::vector<std::string>& getAssetsEntries() { return m_assetsEntries; }

  static constexpr auto GAME_EXTENSION = ".plagame";
  static constexpr auto BOARD_DESCRIPTION_FILE = "BoardDescription.json";
  static constexpr auto GAMES_DIR = "scripts/games/";
  static constexpr auto DEFAULT_ASSETS_DIR = "assets";
  static constexpr auto ASSETS_DIR = "Assets/";
  static constexpr auto LUA_SCRIPT_EXTENSION = ".lua";
  static constexpr auto LUA_SCRIPT_INIT_SUFFIX = "-init.lua";
private:
  void _getAssetsList();

  const std::string& m_gameName;

  ZipArchive::Ptr m_plagameFile;

  std::vector<std::string> m_assetsEntries;
};

} // namespace
