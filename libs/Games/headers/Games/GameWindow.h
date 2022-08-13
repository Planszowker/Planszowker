#pragma once

#include <SFML/Graphics.hpp>

#include <SFGUI/SFGUI.hpp>

namespace pla::games {

class GameWindow : public sf::RenderWindow
{
public:
  using sf::RenderWindow::RenderWindow; // Use sf::RenderWindow constructors

  ~GameWindow() override = default;
};

} // namespaces
