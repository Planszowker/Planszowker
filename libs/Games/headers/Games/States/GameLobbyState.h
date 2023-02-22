#pragma once

#include "IState.h"

#include <Games/Callbacks/ICallbacks.h>
#include <Games/GameWindow.h>
#include <GamesClient/GraphicalView.h>
#include <NetworkHandler/ClientPacketHandler.h>
#include <Games/GamesMetaInfo.h>

#include <imgui.h>
#include <imgui-SFML.h>

#include <vector>

namespace pla::games {

struct GameLobbyStateArguments
{
  std::string gameName;
};

enum class LobbyState
{
  Main,
  CreateLobby,
  JoinLobby,
  LobbyDetails
};

class GameLobbyState final : public IState
{
public:
  GameLobbyState() = delete;
  explicit GameLobbyState(games_client::GraphicalView& graphicalView, GameLobbyStateArguments gameLobbyStateArguments);

  void eventHandling() final;
  void display() final;
  void init() final;

  void updateLobbyDetails(const nlohmann::json& updateJson);
  void updateLobbiesList(const nlohmann::json& updateJson);
private:
  void _guiDisplayMainGui();
  void _guiDisplayCreateNewLobby();
  void _guiDisplayJoinLobby();
  void _guiDisplayLobby();

  games_client::GraphicalView& m_graphicalView;
  games_client::Controller& m_controller;
  GameWindow& m_gameWindow;

  std::shared_ptr<ICallbacks> m_callbacks;

  GameLobbyStateArguments m_gameArguments;

  LobbyState m_lobbyState = LobbyState::Main;

  sf::Clock m_deltaClock;

  std::string m_createNewLobbyErrorString; /// String with error to be displayed when creating new lobby.

  nlohmann::json m_lobbyDetailsJson; ///< JSON to hold information about specific lobby's details.
  nlohmann::json m_lobbiesListJson; ///< JSON to hold information about available lobbies for given game key.
};

} // namespace
