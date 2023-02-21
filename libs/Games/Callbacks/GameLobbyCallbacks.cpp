#include <Callbacks/GameLobbyCallbacks.h>

#include <easylogging++.h>

namespace pla::games {

void GameLobbyCallbacks::IDCallback()
{
  LOG(DEBUG) << "[GameLobbyCallbacks]::IDCallback";
  m_state.
}


void GameLobbyCallbacks::createLobbyCallback(std::any &arg)
{
  LOG(DEBUG) << "[GameLobbyCallbacks]::createLobbyCallback";
}

}