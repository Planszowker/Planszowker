#pragma once

/* Planszowker specific */
#include <Games/States/IState.h>

#include <Games/GameWindow.h>
#include <Games/BoardParser.h>
#include <GamesClient/GraphicalView.h>
#include <Games/Objects/ActionButton.h>
#include <Games/Objects/Entity.h>
#include <Games/Objects/DestinationPoint.h>

/* SFML */
#include <SFML/Graphics.hpp>

/* STD */
#include <map>
#include <vector>

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

  struct EntitySpriteStruct {
    std::shared_ptr<sf::Sprite> spritePtr;
    std::shared_ptr<Entity> entityPtr;
  };
  void _updateSprite(const EntitySpriteStruct& entitySpriteStruct);

  void _updatePlayersPoints();
  void _updateEventLog();

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

  std::vector<EntitySpriteStruct> m_entitiesSprites;

  std::shared_ptr<BoardParser> m_boardParser;

  std::map<std::string, size_t> m_playersPoints; ///< Container to hold players' points.
  std::vector<std::string> m_events; ///< Container to hold events with initial size (100).
};

} // namespace
