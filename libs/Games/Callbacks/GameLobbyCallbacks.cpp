#include <Callbacks/GameLobbyCallbacks.h>

#include <easylogging++.h>

namespace pla::games {

void GameLobbyCallbacks::createLobbyCallback(std::any &arg)
{
  LOG(DEBUG) << "[GameLobbyCallbacks]::createLobbyCallback";
}

}