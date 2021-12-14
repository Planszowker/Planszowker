#pragma once

/* Generic */
#include "Controller.h"
#include "GenericView.h"

/* STD */
#include <functional>
#include <any>
#include <atomic>

/* SFML */
#include <SFML/Graphics.hpp>

namespace pla::common::games {

class Controller;

/*!
 * @brief Graphical view class.
 */
class GraphicalView : public GenericView
{
public:
  explicit GraphicalView(sf::RenderWindow& window);

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
  virtual void _eventHandler();

  /*!
   * @brief Display function. By default it only clears and render window.
   * User should override this method to suit his needs.
   */
  virtual void _display();

  sf::RenderWindow& m_window;
};

} // namespaces
