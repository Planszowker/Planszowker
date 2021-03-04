#include "DiceRollerConsoleView.h"

#include "DiceRoller/DiceRollerVersion.h"
#include "DiceRoller/DiceRollerConsoleViewCallbackObject.h"

#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace pla::common::games;

namespace pla::common::games::dice_roller {

void DiceRollerConsoleView::viewObject(ViewObject &object) {
  // [TODO]: Implementation
}


void DiceRollerConsoleView::init() {
  cout << "DiceRoller v" << DiceRollerVersionMajor << "." << DiceRollerVersionMinor << "." << DiceRollerVersionPatch << "\n";
  cout << "========================================\n";
  cout << "Press any key when it is your turn to roll a dice. Points are summed. Player with bigger score wins.\n";
  cout << "========================================\n";
}


void DiceRollerConsoleView::notifyController(std::function<void(std::any)> callback) {
  DiceRollerConsoleViewCallbackObject callbackObject = {true};

  callback(std::make_any<DiceRollerConsoleViewCallbackObject>(callbackObject));
}


[[noreturn]] void DiceRollerConsoleView::runLoop(Controller* controller) // TODO: Delete noreturn
{
  while (true) // TODO: Replace this infinite loop with some atomic bool checking
  {
    // Wait for input
    cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
    DiceRollerConsoleViewCallbackObject callbackObject = {true};

    std::function<void(std::any)> callback = std::bind(&Controller::viewCallback, controller, std::placeholders::_1);
    notifyController(callback);

    //std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}
#pragma clang diagnostic pop


} // namespaces
