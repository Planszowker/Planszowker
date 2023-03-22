#pragma once

#include <unordered_map>
#include <string>
#include <filesystem>

#include <ZipLib/ZipFile.h>

namespace pla::games_server {

class GamesHandler {
public:
  using AssetsContainer = std::unordered_map<std::string, std::string>; // Assets name, asset type

  static constexpr auto GAME_EXTENSION = ".plagame";
  static constexpr auto BOARD_DESCRIPTION_FILE = "BoardDescription.json";
  static constexpr auto GAMES_DIR = "scripts/games/";
  static constexpr auto DEFAULT_ASSETS_DIR = "assets";
  static constexpr auto ASSETS_DIR = "Assets/";
  static constexpr auto LUA_SCRIPT_EXTENSION = ".lua";
  static constexpr auto LUA_SCRIPT_INIT_SUFFIX = "-init.lua";

  explicit GamesHandler(std::string gameName);

  ZipArchive::Ptr getPlagameFile() { return m_plagameFile; }

  AssetsContainer getAssetsEntries() { return m_assetsEntries; }
private:
  void _getAssetsList();

  std::string m_gameName;
  ZipArchive::Ptr m_plagameFile;
  AssetsContainer m_assetsEntries;
};

} // namespace
