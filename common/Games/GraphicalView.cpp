#include "GraphicalView.h"

namespace pla::common::games {

GraphicalView::GraphicalView(const sf::Vector2i& windowDim, const std::string& windowName)
  : m_gameWindow(std::make_unique<game_utils::GameWindow>(sf::VideoMode(windowDim.x, windowDim.y, 32), windowName))
{
  m_gameWindow->resetGLStates(); // Reset for SFGUI, since it uses OpenGL for rendering

  // Todo: maybe this hard-coded FPS limit is not a good idea?
  m_gameWindow->setFramerateLimit(60);

  // Divide area into two panels
  // _________________________________
  // |                      | PLAYER |
  // |       GAME AREA      |  AREA  |
  // |                      |        |
  // |                      |        |
  // |______________________|________|
  // |<=========0.8========>|<==0.2=>|

  constexpr float gameArea = 0.8f;
  constexpr float playersArea = 0.2f;

  sf::Vector2f gameAreaViewDim {static_cast<float>(windowDim.x) * gameArea, static_cast<float>(windowDim.y)};
  sf::Vector2f playersAreaViewDim {static_cast<float>(windowDim.x) * playersArea, static_cast<float>(windowDim.y)};

  m_gameView.reset(sf::FloatRect(0.f, 0.f, gameAreaViewDim.x, gameAreaViewDim.y));
  m_playersView.reset(sf::FloatRect(0.f, 0.f, playersAreaViewDim.x, playersAreaViewDim.y));

  m_gameView.setViewport(sf::FloatRect(0.f, 0.f, gameArea, 1.f));
  m_playersView.setViewport(sf::FloatRect(gameArea, 0.f, playersArea, 1.f));
}

void GraphicalView::run()
{
  while (m_gameWindow->isOpen())
  {
    // Process events
    sf::Event event{};
    while (m_gameWindow->pollEvent(event))
    {
      // Close window: exit
      if (event.type == sf::Event::Closed) {
        m_gameWindow->close();
      }

      // Handle custom events
      _eventHandler(event);

      m_desktop.HandleEvent(event);
    }
    m_desktop.Update(1.f / 60.f); // Todo: Any possible problems with this?

    m_sfgui.Display(*m_gameWindow);

    m_gameWindow->clear();

    // Display custom objects
    _display();

    m_gameWindow->setView(m_gameWindow->getDefaultView());
    m_gameWindow->display();
  }
}

} // namespaces
