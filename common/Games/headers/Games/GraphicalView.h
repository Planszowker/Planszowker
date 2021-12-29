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

  void _recalculateActionBoxWindowSize();

  // SFML views
  sf::View m_mainWindowView;
  sf::View m_gameAreaView;

  // SFML window
  std::unique_ptr<game_utils::TextureLoader> m_gameWindow;

  // SFGUI main objects
  sfg::SFGUI m_sfgui;
  sfg::Desktop m_desktop;

  // We use 4 SFGUI windows to make a layout
  sfg::Window::Ptr m_gameAreaWindow;
  sfg::Window::Ptr m_playerAreaWindow;
  sfg::Window::Ptr m_logAreaWindow;
  sfg::Window::Ptr m_actionsAreaWindow;

  // Game area
  sfg::Canvas::Ptr m_gameAreaSfmlCanvas;

  // Other areas
  sfg::Box::Ptr m_playerAreaBox;
  sfg::Box::Ptr m_logAreaBox;
  sfg::Box::Ptr m_actionAreaBox;

  // Constants defining window split
  static constexpr float GameAreaWidthFactor = 0.8f;
  static constexpr float PlayerAndLogAreaWidthFactor = 0.2f;
  static constexpr float LogAreaHeightFactor = 0.4f;
  static constexpr float ActionsAreaHeightFactor = 0.1f;
};

} // namespaces
