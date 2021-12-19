#pragma once

/* Generic */
#include "Games/GraphicalView.h"

/* STD */
#include <any>
#include <mutex>
#include <atomic>

/* SFML */
#include <SFML/Graphics.hpp>

/* SFGUI */
#include <SFGUI/Widgets.hpp>

namespace pla::common::games::dice_roller {

class DiceRollerGraphicalView : public GraphicalView
{
public:
  DiceRollerGraphicalView(const sf::Vector2i& windowDim, const std::string& windowName);
  virtual ~DiceRollerGraphicalView() = default;

  void init() final;
  void update(const std::any& objectFromController) final;

private:
  void _eventHandler(sf::Event& event) final;
  void _display() final;

  // GUI elements
  std::shared_ptr<sfg::Button> m_buttonRollReroll;
  std::shared_ptr<sfg::Button> m_buttonConfirm;

  std::vector<std::shared_ptr<sfg::Widget>> m_widgets;
};

} // namespaces
