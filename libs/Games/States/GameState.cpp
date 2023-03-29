#include <States/GameState.h>

#include <Callbacks/GameCallbacks.h>
#include <GamesClient/SharedObjects.h>
#include <Games/CommObjects.h>

#include <Games/Objects/ActionButton.h>

namespace pla::games {

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
  sf::Vector2f gameAreaDim {static_cast<float>(m_gameWindow.getSize().x) * GameAreaWidthFactor, static_cast<float>(m_gameWindow.getSize().y)};
  //sf::Vector2f playersAreaDim {static_cast<float>(m_gameWindow.getSize().x) * PlayerAndLogAreaWidthFactor, static_cast<float>(m_gameWindow.getSize().y) * (1.f - LogAreaHeightFactor)};
  //sf::Vector2f logAreaDim {static_cast<float>(m_gameWindow.getSize().x) * PlayerAndLogAreaWidthFactor, static_cast<float>(m_gameWindow.getSize().y) * LogAreaHeightFactor};

  m_gameAreaView.reset(sf::FloatRect(0.f, 0.f, gameAreaDim.x, gameAreaDim.y));
  m_gameAreaView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
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

  // TODO: Remove this
  ImGui::ShowDemoWindow();

  _actionAreaDisplay();
  _gameAreaDisplay();
  _playerAreaDisplay();
  _logAreaDisplay();

  m_gameWindow.clear(sf::Color(0x54, 0x54, 0x54));

  ImGui::SFML::Render(m_gameWindow);
  m_gameWindow.display();
}


void GameState::_actionAreaDisplay()
{
  // Action area uses ImGui to display window with appropriate action buttons.
  ImGui::Begin("Action window");

  if (m_boardParser) {
    for (auto& [actionButtonName, objectPtr] : m_boardParser->getActionButtons()) {
      auto actionButtonPtr = std::dynamic_pointer_cast<ActionButton>(objectPtr);
      auto params = actionButtonPtr->getParams();
      if (params.visible) {
        ImGui::Button(params.displayName.c_str());
      }
    }
  }

  ImGui::End();
}


void GameState::_gameAreaDisplay()
{
  // Game area uses SFML (may be using ImGui as well) to display and handle sprites.

  // TODO
}


void GameState::_playerAreaDisplay()
{
  // Player area uses ImGui to display information about the players (points, round counter, etc.).
  // TODO
}


void GameState::_logAreaDisplay()
{
  // Log area uses ImGui to display game logs and maybe chat messages.
  // TODO
}

} // namespace
