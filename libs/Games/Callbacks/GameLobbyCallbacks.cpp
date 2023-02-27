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


void GameLobbyCallbacks::joinLobbyCallback(const std::any& arg)
{
  LOG(DEBUG) << "[GameLobbyCallbacks]::joinLobbyCallback";
  try {
    auto replyJson = nlohmann::json::parse(std::any_cast<std::string>(arg));
    if(replyJson["Valid"].get<bool>()) {
      m_state.m_lobbyState = LobbyState::LobbyDetails;
    }
  } catch (const std::exception& e) {
  }
}


void GameLobbyCallbacks::createLobbyCallback(const std::any& arg)
{
  LOG(DEBUG) << "[GameLobbyCallbacks]::createLobbyCallback";

  try {
    auto replyJson = nlohmann::json::parse(std::any_cast<std::string>(arg));
    if(replyJson["Valid"].get<bool>()) {
      m_state.m_lobbyState = LobbyState::LobbyDetails;
    }
  } catch (const std::exception& e) {
  }
}

}