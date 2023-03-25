#pragma once

#include <Games/States/IState.h>

#include <Games/GameWindow.h>
#include <Games/BoardParser.h>
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
  void _actionAreaDisplay();
  void _logAreaDisplay();
  void _playerAreaDisplay();
  void _gameAreaDisplay();

  friend class GameCallbacks;

  games_client::GraphicalView& m_graphicalView;
  games_client::Controller& m_controller;
  GameWindow& m_gameWindow;

  std::shared_ptr<ICallbacks> m_callbacks;

  GameStateArguments m_gameArguments;

  // State specific parameters

  // SFML views
  sf::View m_mainWindowView;
  sf::View m_gameAreaView;

  // Constants defining window split
  static constexpr float GameAreaWidthFactor = 0.8f;
  static constexpr float PlayerAndLogAreaWidthFactor = 0.2f;
  static constexpr float LogAreaHeightFactor = 0.4f;
  static constexpr float ActionsAreaHeightFactor = 0.1f;

  sf::Clock m_deltaClock;

  std::shared_ptr<BoardParser> m_boardParser;
};

} // namespace
