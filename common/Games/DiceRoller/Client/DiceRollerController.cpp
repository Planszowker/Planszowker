#include "DiceRollerController.h"

#include "DiceRollerConsoleView.h"
#include "DiceRoller/DiceRollerConsoleViewCallbackObject.h"
#include "Logger/Logger.h"
#include "ErrorHandler/ErrorLogger.h"

#include <thread>
#include <chrono>

using namespace pla::common::err_handler;
using namespace pla::common::logger;

namespace pla::common::games::dice_roller {

void DiceRollerController::run()
{
  if (m_view == nullptr) {
    ErrorLogger::printError("View is null!");
    return;
  }

  // Initialize view - print some basic info
  m_view->init();

  std::thread viewInput(&DiceRollerConsoleView::runLoop, m_view, this);

  while (true)
  {
    if (m_view != nullptr) {


      std::this_thread::sleep_for(std::chrono::seconds(2));
    }
  }

  viewInput.join();
}


void DiceRollerController::updateModel()
{
  // TODO
}


void DiceRollerController::updateView()
{

}


void DiceRollerController::viewCallback(std::any object)
{
  auto callbackObject = std::any_cast<DiceRollerConsoleViewCallbackObject>(object);

  if (callbackObject.occurred) {
    Logger::printDebug("Callback from ConsoleView has occurred!");
  } else {
    Logger::printDebug("Callback has occurred, but data got corrupted!");
  }
}


void DiceRollerController::getUpdatedModel()
{
  // TODO
}


void DiceRollerController::attachView(ConsoleView* view) {
  m_view = dynamic_cast<DiceRollerConsoleView*>(view);
}

} // namespace
