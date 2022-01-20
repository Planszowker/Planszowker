#include "GraphicalView.h"

namespace pla::common::games {

void GraphicalView::runLoop(Controller* controller, std::atomic_bool& runLoop)
{
  while (m_window.isOpen())
  {
    // Process events
    sf::Event event;
    while (m_window.pollEvent(event))
    {
      // Close window: exit
      if (event.type == sf::Event::Closed) {
        m_window.close();
      }

      //update();
    }
  }
}

} // namespaces
