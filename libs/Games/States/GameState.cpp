#include <States/GameState.h>

#include <Callbacks/GameCallbacks.h>
#include <GamesClient/SharedObjects.h>
#include <Games/CommObjects.h>
#include <AssetsManager/AssetsReceiver.h>

#include <easylogging++.h>

namespace pla::games {

using namespace assets;

GameState::GameState(games_client::GraphicalView& graphicalView, GameStateArguments gameStateArguments)
  : m_graphicalView(graphicalView)
  , m_gameWindow(*graphicalView.getGameWindow())
  , m_controller(graphicalView.getController())
  , m_gameArguments(std::move(gameStateArguments))
  , m_callbacks(std::make_shared<GameCallbacks>(*this))
{
  // Connect GameChoosing callbacks to packet handler
  m_controller.getPacketHandler()->connectCallbacks(m_callbacks.get());

  // Divide area into panels
  // _________________________________
  // |                      | PLAYER |  / \
  // |    (ACTIONS AREA)    |  AREA  |  0.6
  // |       GAME AREA      |--------|  ---
  // |                      |LOG AREA|  0.4
  // |______________________|________|  \ /
  // |<=========0.8========>|<==0.2=>|

  // Set dimensions for every area
  m_gameAreaDim = sf::Vector2f{static_cast<float>(m_gameWindow.getSize().x) * GameAreaWidthFactor, static_cast<float>(m_gameWindow.getSize().y)};
  m_playersAreaDim = sf::Vector2f{static_cast<float>(m_gameWindow.getSize().x) * PlayerAndLogAreaWidthFactor, static_cast<float>(m_gameWindow.getSize().y) * (1.f - LogAreaHeightFactor)};
  m_logAreaDim = sf::Vector2f{static_cast<float>(m_gameWindow.getSize().x) * PlayerAndLogAreaWidthFactor, static_cast<float>(m_gameWindow.getSize().y) * LogAreaHeightFactor};

  m_gameAreaView.reset(sf::FloatRect(0.f, 0.f, m_gameAreaDim.x, m_gameAreaDim.y));
  m_gameAreaView.setViewport(sf::FloatRect(0.f, 0.f, 0.8f, 1.f));
  m_mainWindowView.reset(sf::FloatRect(0.f, 0.f, static_cast<float>(m_gameWindow.getSize().x), static_cast<float>(m_gameWindow.getSize().y)));
  m_mainWindowView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
}


void GameState::init()
{
  // Download all game's data
  m_controller.sendRequest(PacketType::DownloadAssets);
}


void GameState::eventHandling()
{
  // Process events
  sf::Event event{};
  while (m_gameWindow.pollEvent(event))
  {
    // Handle ImGui events
    ImGui::SFML::ProcessEvent(m_gameWindow, event);

    // Close window: exit
    if (event.type == sf::Event::Closed) {
      m_gameWindow.close();
      m_graphicalView.stop();
      break;
    }
  }
}


void GameState::display()
{
  ImGui::SFML::Update(m_gameWindow, m_deltaClock.restart());

  _init();

  m_gameWindow.clear(sf::Color(0x54, 0x54, 0x54));

  _actionAreaDisplay();
  _gameAreaDisplay();
  _playerAreaDisplay();
  _logAreaDisplay();

  ImGui::SFML::Render(m_gameWindow);
  m_gameWindow.display();
}


void GameState::_init()
{
  // Initiate game board with entities sprites
  if (m_boardParser && not initDone) {
    for (auto [objectId, entityObjectPtr] : m_boardParser->getEntities()) {
      std::shared_ptr<Entity> _entityPtr = dynamic_pointer_cast<Entity>(entityObjectPtr);

      auto entityParams = _entityPtr->getParams();
      auto _texture = assets::AssetsReceiver::getTexture(entityParams.texture);
      if (!_texture) {
        LOG(ERROR) << "Texture for entity id " << objectId << " could not be found! Check BoardDescription file!";
        throw;
      }
      auto _spritePtr = std::make_shared<sf::Sprite>();
      _spritePtr->setTexture(*_texture);

      auto newSize = _convertToAbsolutePosition(entityParams.size);
      auto newFactor = newSize.x / _spritePtr->getGlobalBounds().width;
      _spritePtr->setOrigin(_spritePtr->getGlobalBounds().width / 2.0, _spritePtr->getGlobalBounds().height / 2.0);
      _spritePtr->setScale(newFactor, newFactor);

      const auto& destinationPointMap = m_boardParser->getDestinationPoints();
      auto it = destinationPointMap.find(entityParams.positionAsDestinationPoint);
      if (it == m_boardParser->getDestinationPoints().end()) {
        LOG(ERROR) << "Destination point " << entityParams.positionAsDestinationPoint << " for entity id " << objectId << " could not be found! Check BoardDescription file!";
        throw;
      }
      auto destinationPointPtr = dynamic_pointer_cast<DestinationPoint>(it->second);
      _spritePtr->setPosition(_convertToAbsolutePosition(destinationPointPtr->getParams().position));

      EntitySpriteStruct entitySprite{
        .spritePtr = std::move(_spritePtr),
        .entityPtr = _entityPtr,
      };

      m_entitiesSprites.push_back(entitySprite);
    }
    initDone = true;
  }
}


void GameState::_actionAreaDisplay()
{
  // Action area uses ImGui to display window with appropriate action buttons.
  ImGui::SetNextWindowSize(ImVec2(m_gameAreaView.getSize().x - 2.f * ActionWindowWidthPadding,
                                       ActionWindowHeight),
                                    ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowPos(ImVec2(ActionWindowWidthPadding, ActionWindowHeightPadding), ImGuiCond_FirstUseEver);
  ImGui::Begin("Action window", nullptr, ImGuiWindowFlags_NoTitleBar |
                                                            ImGuiWindowFlags_NoCollapse |
                                                            ImGuiWindowFlags_NoResize |
                                                            ImGuiWindowFlags_NoMove);

  if (m_boardParser) {
    auto actionButtonsMap = m_boardParser->getActionButtons();

    auto actionButtonsSize = static_cast<float>(std::count_if(actionButtonsMap.begin(), actionButtonsMap.end(), [](auto param){
      auto ptr = dynamic_pointer_cast<ActionButton>(param.second);
      return ptr->getParams().visible;
    }));

    for (auto& [actionButtonName, objectPtr] : actionButtonsMap) {
      auto actionButtonPtr = std::dynamic_pointer_cast<ActionButton>(objectPtr);
      auto params = actionButtonPtr->getParams();
      if (params.visible && ImGui::Button(params.displayName.c_str(), ImVec2(ImGui::GetWindowContentRegionWidth() / actionButtonsSize, -FLT_MIN))) {
        m_boardParser->performUpdateAndSendToServer(*m_controller.getPacketHandler(), actionButtonPtr, UpdateActions::ButtonPressed);
      }
    }
  }

  ImGui::End();
}


void GameState::_gameAreaDisplay()
{
  // Game area uses SFML (perhaps using ImGui as well) to display and handle sprites.
  m_gameWindow.setView(m_gameAreaView);

  // We should iterate over all entities and determine whether they should be displayed.

  if (m_boardParser) {
    for (const auto& entitySprite : m_entitiesSprites) {
      if (m_boardParser->isMarkedForUpdate()) {
        _updateSprite(entitySprite);
      }

      if (entitySprite.entityPtr->getParams().visible) {
        m_gameWindow.draw(*entitySprite.spritePtr);

        // TODO: Remove it later - used fo debugging entities
        sf::RectangleShape ziu;
        ziu.setSize(sf::Vector2f{40.f, 40.f});
        ziu.setOrigin(ziu.getGlobalBounds().width/2, ziu.getGlobalBounds().height/2);
        ziu.setOutlineThickness(5);
        ziu.setFillColor(sf::Color(255, 255, 255, 120));
        ziu.setOutlineColor(sf::Color(0, 100, 100, 120));
        ziu.setPosition(entitySprite.spritePtr->getPosition());
        m_gameWindow.draw(ziu);
      }
    }

    if(m_boardParser->isMarkedForUpdate()) {
      m_boardParser->markBoardUpdated();
    }
  }

  m_gameWindow.setView(m_mainWindowView);
}

void GameState::_playerAreaDisplay()
{
  // Player area uses ImGui to display information about the players (points, round counter, etc.).
  ImGui::SetNextWindowSize(ImVec2(m_playersAreaDim.x, m_playersAreaDim.y),
                           ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowPos(ImVec2(m_gameAreaDim.x, 0.f), ImGuiCond_FirstUseEver);
  ImGui::Begin("Player window", nullptr, // ImGuiWindowFlags_NoTitleBar |
                                                           ImGuiWindowFlags_NoCollapse |
                                                           ImGuiWindowFlags_NoResize |
                                                           ImGuiWindowFlags_NoMove);

  ImGui::End();
}


void GameState::_logAreaDisplay()
{
  // Log area uses ImGui to display game logs and maybe chat messages.
  ImGui::SetNextWindowSize(ImVec2(m_logAreaDim.x, m_logAreaDim.y),
                           ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowPos(ImVec2(m_gameAreaDim.x, m_playersAreaDim.y), ImGuiCond_FirstUseEver);
  ImGui::Begin("Log window", nullptr, // ImGuiWindowFlags_NoTitleBar |
                                         ImGuiWindowFlags_NoCollapse |
                                         ImGuiWindowFlags_NoResize |
                                         ImGuiWindowFlags_NoMove);

  ImGui::End();
}


void GameState::_updateSprite(const EntitySpriteStruct& entitySpriteStruct)
{
  auto _entityParams = entitySpriteStruct.entityPtr->getParams();
  auto& _spritePtr = entitySpriteStruct.spritePtr;

  auto _updatedTexture = assets::AssetsReceiver::getTexture(_entityParams.texture);
  if (!_updatedTexture) {
    LOG(ERROR) << "Texture for entity id " << _entityParams.id << " could not be updated!";
    throw;
  }

  // Update texture
  _spritePtr->setTexture(*_updatedTexture);

  // Update size
  auto newSize = _convertToAbsolutePosition(_entityParams.size);
  auto newFactor = newSize.x / _spritePtr->getGlobalBounds().width;
  _spritePtr->setOrigin(_spritePtr->getGlobalBounds().width / 2.0, _spritePtr->getGlobalBounds().height / 2.0);
  _spritePtr->setScale(newFactor, newFactor);

  const auto& destinationPointMap = m_boardParser->getDestinationPoints();
  auto it = destinationPointMap.find(_entityParams.positionAsDestinationPoint);
  if (it == m_boardParser->getDestinationPoints().end()) {
    LOG(ERROR) << "Destination point " << _entityParams.positionAsDestinationPoint << " for entity id " << _entityParams.positionAsDestinationPoint << " could not be updated!";
    throw;
  }
  auto destinationPointPtr = dynamic_pointer_cast<DestinationPoint>(it->second);
  _spritePtr->setPosition(_convertToAbsolutePosition(destinationPointPtr->getParams().position));
}


sf::Vector2f GameState::_convertToAbsolutePosition(const sf::Vector2f& vec)
{
  // Input is a vector of values between 0.0 and 1.0.
  // We have to convert to absolute values of available screen.
  auto availableScreenSize = m_gameAreaView.getSize();

  return sf::Vector2f {availableScreenSize.x * vec.x, availableScreenSize.y * vec.y};
}

} // namespace
