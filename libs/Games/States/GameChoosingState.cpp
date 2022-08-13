#include <States/GameChoosingState.h>

#include <Games/Callbacks/GameChoosingCallbacks.h>

namespace pla::games {

GameChoosingState::GameChoosingState(games_client::GraphicalView& graphicalView)
  : m_graphicalView(graphicalView)
  , m_gameWindow(*graphicalView.getGameWindow())
  , m_controller(graphicalView.getController())
  , m_callbacks(std::make_shared<GameChoosingCallbacks>())
{
  // Connect GameChoosing callbacks to packet handler
  m_controller.getPacketHandler()->connectCallbacks(m_callbacks.get());
}


void GameChoosingState::init()
{
  m_controller.sendRequest(games::PacketType::ID);
  m_controller.sendRequest(games::PacketType::ListAvailableGames);
}


void GameChoosingState::eventHandling()
{
  // Process events
  sf::Event event{};
  while (m_gameWindow.pollEvent(event))
  {
    // Close window: exit
    if (event.type == sf::Event::Closed) {
      m_gameWindow.close();
      m_graphicalView.stop();
      break;
    }

    // TODO: Remove it later
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G) {
      m_controller.sendRequest(games::PacketType::ListAvailableGames);
    }
  }
}

void GameChoosingState::display()
{
  m_gameWindow.clear(sf::Color::Yellow);
  m_gameWindow.display();
}

} // namespace
