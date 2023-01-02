#pragma once

/* Generic */
#include "Controller.h"
#include "FontManager.h"

#include <Games/GameWindow.h>
#include <Games/States/IState.h>

#include <imgui.h>
#include <imgui-SFML.h>

/* STD */
#include <functional>
#include <any>
#include <atomic>
#include <deque>
#include <memory>

namespace pla::games_client {

class Controller;

/*!
 * @brief Graphical view class. It is responsible to create a window and render all provided data.
 */
class GraphicalView
{
public:
  GraphicalView() = delete;
  explicit GraphicalView(Controller& controller, std::atomic_bool& run, const sf::Vector2i& windowDim, const std::string& windowName);

  ~GraphicalView();

  void init();

  /*!
   * @brief Run view in loop (get user input, handle events, send callbacks).
   */
  void run();

  void stop()
  {
    m_run = false;
  }

  const std::unique_ptr<games::GameWindow>& getGameWindow()
  {
    return m_gameWindow;
  }

  Controller& getController()
  {
    return m_controller;
  }

  const FontManager& getFontManager() const
  {
    return m_fontManager;
  }

private:
  void changeState(games::States newState);

  Controller& m_controller;
  FontManager m_fontManager;

  std::deque<std::shared_ptr<games::IState>> m_states;

  std::atomic_bool& m_run;

  // Game window
  std::unique_ptr<games::GameWindow> m_gameWindow;
};

} // namespaces
