#include "Callbacks/GameChoosingCallbacks.h"

// TODO: Remove this debug printouts...
#include <iostream>

namespace pla::games {

void GameChoosingCallbacks::IDCallback()
{
  std::cout << "[GameChoosingCallbacks] ID Callback invoked!\n";
}


void GameChoosingCallbacks::listAllAvailableGamesCallback()
{
  std::cout << "[GameChoosingCallbacks] List All Available Games Callback invoked!";
}

}