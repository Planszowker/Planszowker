#include <States/GameLobbyState.h>

#include <easylogging++.h>

namespace pla::games {

GameLobbyState::GameLobbyState(games_client::GraphicalView& graphicalView)
  : m_graphicalView(graphicalView)
  , m_gameWindow(*graphicalView.getGameWindow())
  , m_controller(graphicalView.getController())
{
}


void GameLobbyState::init()
{
  LOG(DEBUG) << "[GameLobbyState] init";
}


void GameLobbyState::eventHandling()
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


void GameLobbyState::display()
{
  ImGui::SFML::Update(m_gameWindow, m_deltaClock.restart());

  // TODO: Test-purpose button only
  if(ImGui::Button("Back"))
  {
    m_graphicalView.changeState(States::GameChoosing);
  }


  //m_gameWindow.clear(sf::Color(55, 55, 55));
  m_gameWindow.clear(sf::Color::Magenta);

  ImGui::SFML::Render(m_gameWindow);
  m_gameWindow.display();
}

} // namespace
