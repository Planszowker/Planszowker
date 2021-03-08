#include "ViewLogic.h"

#include "ErrorHandler/ErrorLogger.h"
#include "DiceRoller/NetworkObjects.h"

namespace pla::common::games::dice_roller {

using namespace err_handler;

void DiceRollerViewLogic::update(std::any& object)
{
  try {
    auto updateObject = std::any_cast<DiceRollerReplyFromServer>(object);
  } catch (const std::exception& e) {
    ErrorLogger::printWarning("Cannot cast received object!");
  }

  updateView();
}

void DiceRollerViewLogic::updateView()
{
  DiceRollerReplyFromServer toView;
  toView.additionalInfo = "Additional info from ViewLogic ;)";
  toView.reply = ServerReplies::Success;

  if (m_view != nullptr) {
    m_view->update(std::make_any<DiceRollerReplyFromServer>(toView));
  }
}

} // namespaces
