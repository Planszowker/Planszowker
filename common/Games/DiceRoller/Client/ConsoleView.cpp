#include "ConsoleView.h"

#include "DiceRoller/Version.h"
#include "DiceRoller/ConsoleViewCallbackObject.h"
#include "DiceRoller/NetworkObjects.h"

#include <iostream>
#include <chrono>
#include <string>
#include <atomic>

using namespace pla::common::games;

namespace pla::common::games::dice_roller {

void DiceRollerConsoleView::update(const std::any& object) {
  auto receivedObject = std::any_cast<DiceRollerReplyFromServer>(object);

  // TODO: Remove below printout...
  std::cout << "Received additional info: " << receivedObject.additionalInfo << "\n";
  std::cout << "Reply: " << static_cast<size_t>(receivedObject.reply) << "\n";
}


void DiceRollerConsoleView::init() {
  // No mutex is needed here, since it is invoked before any multithreading

  // Some basic info about a game
  cout << "DiceRoller v" << DiceRollerVersionMajor << "." << DiceRollerVersionMinor << "." << DiceRollerVersionPatch << "\n";
  cout << "========================================\n";
  cout << "Press any key when it is your turn to roll dice. Points are summed. Player with a greater score wins.\n";
  cout << "========================================\n";
}


void DiceRollerConsoleView::notifyController(std::function<void(std::any)> callback) {
  DiceRollerConsoleViewCallbackObject callbackObject = {true};

  callback(std::make_any<DiceRollerConsoleViewCallbackObject>(callbackObject));
}


void DiceRollerConsoleView::runLoop(Controller* controller, std::atomic_bool& runLoop)
{
  while (runLoop)
  {
    // Wait for input
    cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
    DiceRollerConsoleViewCallbackObject callbackObject = {true};

    std::function<void(std::any)> callback = std::bind(&Controller::viewCallback, controller, std::placeholders::_1);
    notifyController(callback);
  }
}

} // namespaces
