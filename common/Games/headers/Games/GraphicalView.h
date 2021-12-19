#pragma once

/* Generic */
#include "Controller.h"
#include "GenericView.h"
#include "GameWindow/GameWindow.h"

/* STD */
#include <functional>
#include <any>
#include <atomic>

/* SFML */
#include <SFML/Graphics.hpp>

/* SFGUI */
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

namespace pla::common::games {

class Controller;

/*!
 * @brief Graphical view class.
 */
class GraphicalView : public GenericView
{
public:
  GraphicalView(const sf::Vector2i& windowDim, const std::string& windowName);

  /*!
   * @brief Initialization method. Has to be invoked at first place.
   */
  void init() override = 0;

  /*!
   * @brief Run view in loop (get user input, handle events, send callbacks).
   * User can override this method if needed.
   *
   * @param controller Pointer to a controller.
   * @param runLoop Used for thread syncing.
   */
  void run() override;

protected:
  /*!
   * @brief Handle events. User can override this method for custom event handling.
   */
  virtual void _eventHandler(sf::Event& event) = 0;

  /*!
   * @brief Display function. By default it only clears and render window.
   * User should override this method to suit his needs.
   */
  virtual void _display() = 0;

  // SFML views - split window in game view and player/log view
  sf::View m_gameView;
  sf::View m_playersView;

  std::unique_ptr<game_utils::GameWindow> m_gameWindow;
  sfg::SFGUI m_sfgui;
  sfg::Desktop m_desktop;
};

} // namespaces
