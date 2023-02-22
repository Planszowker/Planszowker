#include <Callbacks/GameLobbyCallbacks.h>

#include <ErrorHandler/ErrorLogger.h>
#include <easylogging++.h>

namespace pla::games {

void GameLobbyCallbacks::getLobbyDetailsCallback(const std::any& arg) {
  LOG(DEBUG) << "[GameLobbyCallbacks]::getLobbyDetailsCallback";
  try {
    auto lobbyDetailsJson = nlohmann::json::parse(std::any_cast<std::string>(arg));
    m_state.updateLobbyDetails(lobbyDetailsJson);
  } catch (std::exception& e) {
    err_handler::ErrorLogger::printError("[GameLobbyCallbacks] Bad any cast!");
  }
}


void GameLobbyCallbacks::listOpenLobbiesCallback(const std::any& arg)
{
  LOG(DEBUG) << "[GameLobbyCallbacks]::listOpenLobbiesCallback";
  try {
    auto lobbiesListJson = nlohmann::json::parse(std::any_cast<std::string>(arg));
    m_state.updateLobbiesList(lobbiesListJson);
  } catch (std::exception& e) {
    err_handler::ErrorLogger::printError("[GameLobbyCallbacks] Bad any cast!");
  }
}

}