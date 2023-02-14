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

class GameLobbyState final : public IState
{
public:
  GameLobbyState() = delete;
  explicit GameLobbyState(games_client::GraphicalView& graphicalView);

  void eventHandling() final;
  void display() final;
  void init() final;
private:
  games_client::GraphicalView& m_graphicalView;
  GameWindow& m_gameWindow;
  games_client::Controller& m_controller;

  //GamesMetaInfo m_gamesMetaInfo;

  sf::Clock m_deltaClock;
};

} // namespace
