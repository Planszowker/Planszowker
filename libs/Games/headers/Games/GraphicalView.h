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

namespace pla::games {

class Controller;

/*!
 * @brief Graphical view class.
 */
class GraphicalView : public GenericView
{
public:
  /*!
   * @brief Initialization method. Has to be invoked at first place.
   */
  void init() override = 0;

  /*!
   * @brief Run view in loop (get user input, handle events, send callbacks).
   *
   * @param controller Pointer to a controller.
   * @param runLoop Used for thread syncing.
   */
  void runLoop(Controller* controller, std::atomic_bool& runLoop) override;

  /*!
   * @brief Update rendering.
   *
   * @param object Game-specific object that holds required data.
   */
  void update(const std::any& object) override = 0;

protected:
  /*!
   * @brief Notify controller about a recent event.
   *
   * @param callback Controller's function that should be invoked.
   */
  void notifyController(std::function<void(std::any&)> callback) override = 0;

  sf::Window m_window; // TODO: This shouldn't be declared here, as main Window will be created in overall GUI
};

} // namespaces
