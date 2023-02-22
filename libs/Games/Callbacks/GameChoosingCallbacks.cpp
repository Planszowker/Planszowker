#include "Callbacks/GameChoosingCallbacks.h"

#include <easylogging++.h>

namespace pla::games {

void GameChoosingCallbacks::listAllAvailableGamesCallback(const std::any& arg)
{
  try {
    m_state.updateAvailableGames(std::any_cast<std::string>(arg));
  } catch (const std::bad_any_cast& e) {
    LOG(DEBUG) << "=== BAD ANY CAST! ===";
  }
}

}