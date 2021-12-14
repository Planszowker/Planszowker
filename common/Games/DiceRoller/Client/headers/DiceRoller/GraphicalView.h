#pragma once

/* Generic */
#include "Games/GraphicalView.h"

/* STD */
#include <any>
#include <mutex>
#include <atomic>

namespace pla::common::games::dice_roller {

class DiceRollerGraphicalView : public GraphicalView
{
public:
  using GraphicalView::GraphicalView;
  virtual ~DiceRollerGraphicalView() = default;

  void init() final;
  void update(const std::any& object);

private:
  void notifyController(std::function<void(std::any&)> callback);

  int m_inputType{0};
};

} // namespaces
