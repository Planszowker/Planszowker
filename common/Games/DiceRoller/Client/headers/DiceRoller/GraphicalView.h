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

  sfg::Button::Ptr m_rollRerollButton;
  sfg::Button::Ptr m_confirmButton;

  static constexpr float MinimalButtonWidth{100.f};
  static constexpr float MinimalButtonHeight{30.f};
};

} // namespaces
