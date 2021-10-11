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
  auto receivedObject = std::any_cast<std::string>(object);

  std::cout << receivedObject << std::endl;

  // TODO: Remove below printout...
  //std::cout << "Received additional info: " << receivedObject.additionalInfo << "\n";
  //std::cout << "Reply: " << static_cast<size_t>(receivedObject.reply) << "\n";
}


void DiceRollerConsoleView::init() {
  // No mutex is needed here, since it is invoked before any multithreading

  // Some basic info about a game
  std::cout << "DiceRoller v" << DiceRollerVersionMajor << "." << DiceRollerVersionMinor << "." << DiceRollerVersionPatch << "\n";
  std::cout << "========================================\n";
  std::cout << "Press any key when it is your turn to roll dice. Points are summed. Player with a greater score wins.\n";
  std::cout << "========================================\n";
}


void DiceRollerConsoleView::notifyController(std::function<void(std::any&)> callback) {
  DiceRollerConsoleViewCallbackObject callbackObject = {true};

  //auto ziu = std::make_any<DiceRollerConsoleViewCallbackObject>(callbackObject);
  auto ziu = std::make_any<bool>(true);

  std::cout << ziu.type().name() << "\n";

  callback(ziu);
}


void DiceRollerConsoleView::runLoop(Controller* controller, std::atomic_bool& runLoop)
{
  while (runLoop)
  {
    // Wait for input
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::function<void(std::any&)> callback = std::bind(&Controller::viewCallback, controller, std::placeholders::_1);
    notifyController(callback);
  }
}

} // namespaces
