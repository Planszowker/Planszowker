#pragma once

#include "IState.h"

#include <Games/GameWindow.h>
#include <GamesClient/GraphicalView.h>

/* SFML */
#include <SFML/Graphics.hpp>

namespace pla::games {

struct GameStateArguments
{
  std::string gameName;
  size_t creatorId;
};

class GameState final : public IState
{
public:
  GameState() = delete;
  GameState(games_client::GraphicalView& graphicalView, GameStateArguments gameStateArguments);

  void eventHandling() final;
  void display() final;
  void init() final;

private:
  games_client::GraphicalView& m_graphicalView;
  games_client::Controller& m_controller;
  GameWindow& m_gameWindow;

  std::shared_ptr<ICallbacks> m_callbacks;

  GameStateArguments m_gameArguments;

  // State specific parameters

  // SFML views
  sf::View m_mainWindowView;
  sf::View m_gameAreaView;

//  TODO: Use ImGui
//  // SFGUI main objects
//  sfg::SFGUI m_sfgui;
//  sfg::Desktop m_desktop;
//
//  // We use 4 SFGUI windows to make a layout
//  sfg::Window::Ptr m_gameAreaWindow;
//  sfg::Window::Ptr m_playerAreaWindow;
//  sfg::Window::Ptr m_logAreaWindow;
//  sfg::Window::Ptr m_actionsAreaWindow;
//
//  // Game area
//  sfg::Canvas::Ptr m_gameAreaSfmlCanvas;
//
//  // Other areas
//  sfg::Box::Ptr m_playerAreaBox;
//  sfg::Box::Ptr m_logAreaBox;
//  sfg::Box::Ptr m_actionAreaBox;

  // Constants defining window split
  static constexpr float GameAreaWidthFactor = 0.8f;
  static constexpr float PlayerAndLogAreaWidthFactor = 0.2f;
  static constexpr float LogAreaHeightFactor = 0.4f;
  static constexpr float ActionsAreaHeightFactor = 0.1f;
};

} // namespace
