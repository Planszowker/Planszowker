#include "GraphicalView.h"

namespace pla::common::games {

GraphicalView::GraphicalView(sf::RenderWindow& window)
  : m_window(window)
{
}

void GraphicalView::run()
{
  m_window.resetGLStates();

  sfg::Desktop desktop;
  desktop.SetProperty( "Button#create_window", "FontSize", 18.f );

  auto button = sfg::Button::Create("Hello");
  button->GetSignal(sfg::Button::OnLeftClick).Connect(
          [this] { OnButtonClick(); }
  );

  desktop.Add(button);

  while (m_window.isOpen())
  {
    // Process events
    sf::Event event{};
    while (m_window.pollEvent(event))
    {
      // Close window: exit
      if (event.type == sf::Event::Closed) {

        m_window.close();
      }
      else {
        desktop.HandleEvent(event);
      }
    }

    desktop.Update(0.f);
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

  m_window.setActive(true);

  sfgui.Display(m_window);

  m_window.display();
}

} // namespaces
