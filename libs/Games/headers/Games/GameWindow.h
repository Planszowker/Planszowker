#pragma once

#include <SFML/Graphics.hpp>

namespace pla::games {

class GameWindow : public sf::RenderWindow
{
public:
  using sf::RenderWindow::RenderWindow; // Use sf::RenderWindow constructors

  ~GameWindow() override = default;
};

} // namespaces
