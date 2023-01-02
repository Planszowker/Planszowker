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

class GameChoosingState final : public IState
{
public:
  GameChoosingState() = delete;
  explicit GameChoosingState(games_client::GraphicalView& graphicalView);

  void eventHandling() final;
  void display() final;
  void init() final;

  void updateAvailableGames(const std::string& combinedString);
private:
  void displayGameTile();

  games_client::GraphicalView& m_graphicalView;
  GameWindow& m_gameWindow;
  games_client::Controller& m_controller;

  std::shared_ptr<ICallbacks> m_callbacks;

  GamesMetaInfo m_gamesMetaInfo;

  sf::Clock m_deltaClock;
};

} // namespace
