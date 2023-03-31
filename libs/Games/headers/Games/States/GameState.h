#pragma once

#include <Games/States/IState.h>

#include <Games/GameWindow.h>
#include <Games/BoardParser.h>
#include <GamesClient/GraphicalView.h>
#include <Games/Objects/ActionButton.h>
#include <Games/Objects/Entity.h>
#include <Games/Objects/DestinationPoint.h>

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
  void _init();
  void _actionAreaDisplay();
  void _logAreaDisplay();
  void _playerAreaDisplay();
  void _gameAreaDisplay();

  sf::Vector2f _convertToAbsolutePosition(const sf::Vector2f& vec);

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

  // Action window defines
  static constexpr float ActionWindowWidthPadding = 80.f;
  static constexpr float ActionWindowHeightPadding = 50.f;
  static constexpr float ActionWindowHeight = 60.f;

  sf::Clock m_deltaClock;

  sf::Vector2f m_gameAreaDim;
  sf::Vector2f m_playersAreaDim;
  sf::Vector2f m_logAreaDim;

  bool initDone {false};

  struct EntitySprite {
    std::shared_ptr<sf::Sprite> spritePtr;
    std::shared_ptr<Entity> entityPtr;
  };

  std::vector<EntitySprite> m_entitiesSprites;

  std::shared_ptr<BoardParser> m_boardParser;
};

} // namespace
