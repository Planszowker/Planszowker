#pragma once

#include <SFML/Graphics.hpp>

#include <SFGUI/SFGUI.hpp>

namespace pla::common::game_utils {

class GameWindow : public sf::RenderWindow
{
public:
  using sf::RenderWindow::RenderWindow; // Use sf::RenderWindow constructors
};

} // namespaces
