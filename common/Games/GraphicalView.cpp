#include "GraphicalView.h"

namespace pla::common::games {

GraphicalView::GraphicalView(sf::RenderWindow& window)
  : m_window(window)
{
}

void GraphicalView::run()
{
  while (m_window.isOpen())
  {
    // Process events
    _eventHandler();

    _display();
  }
}

void GraphicalView::_eventHandler()
{
  sf::Event event{};
  while (m_window.pollEvent(event))
  {
    // Close window: exit
    if (event.type == sf::Event::Closed) {
      m_window.close();
    }
  }
}

void GraphicalView::_display()
{
  m_window.clear();
  m_window.display();
}

} // namespaces
