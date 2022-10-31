#include "Callbacks/GameChoosingCallbacks.h"

#include <easylogging++.h>

namespace pla::games {

void GameChoosingCallbacks::IDCallback()
{
  LOG(DEBUG) << "[GameChoosingCallbacks] ID Callback invoked!";
}


void GameChoosingCallbacks::listAllAvailableGamesCallback()
{
  LOG(DEBUG) << "[GameChoosingCallbacks] List All Available Games Callback invoked!";
}

}