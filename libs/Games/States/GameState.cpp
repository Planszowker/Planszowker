#include <States/GameState.h>

#include <Callbacks/GameCallbacks.h>
#include <GamesClient/SharedObjects.h>
#include <Games/Objects.h>

namespace pla::games {

GameState::GameState(games_client::GraphicalView& graphicalView, GameStateArguments gameStateArguments)
  : m_graphicalView(graphicalView)
  , m_gameWindow(*graphicalView.getGameWindow())
  , m_controller(graphicalView.getController())
  , m_gameArguments(std::move(gameStateArguments))
  , m_callbacks(std::make_shared<GameCallbacks>())
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

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G) {
      sf::Packet packet;

      games::Request request {
        .type = PacketType::GameSpecificData,
      };

      packet << request;
      m_controller.getPacketHandler()->sendPacket(packet);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::J) {
      sf::Packet packet;

      games::Request request {
        .type = PacketType::DownloadAssets,
      };

      packet << request;
      m_controller.getPacketHandler()->sendPacket(packet);
    }
  }
}


void GameState::display()
{
  ImGui::SFML::Update(m_gameWindow, m_deltaClock.restart());

  ImGui::ShowUserGuide();
  ImGui::ShowDemoWindow();

  m_gameWindow.clear(sf::Color(0x54, 0x54, 0x54));

  ImGui::SFML::Render(m_gameWindow);
  m_gameWindow.display();
}

} // namespace
