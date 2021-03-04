#include "DiceRollerConsoleView.h"

#include "DiceRoller/Version.h"
#include "DiceRoller/ConsoleViewCallbackObject.h"

#include <iostream>
#include <chrono>
#include <string>

using namespace std;
using namespace pla::common::games;

namespace pla::common::games::dice_roller {

void DiceRollerConsoleView::update(const std::any& object) {
  auto receivedString = std::any_cast<std::string>(object);

  std::cout << receivedString << "\n";
}


void DiceRollerConsoleView::init() {
  // No mutex is needed here, since it is invoked before any multithreading

  // Some basic info about a game
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
    cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
    DiceRollerConsoleViewCallbackObject callbackObject = {true};

    std::function<void(std::any)> callback = std::bind(&Controller::viewCallback, controller, std::placeholders::_1);
    notifyController(callback);
  }
}

} // namespaces
