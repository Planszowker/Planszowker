#include <States/GameLobbyState.h>
#include <Callbacks/GameLobbyCallbacks.h>

#include <easylogging++.h>
#include <nlohmann/json.hpp>

#include <imgui_stdlib.h>

#include <utility>

namespace pla::games {

GameLobbyState::GameLobbyState(games_client::GraphicalView& graphicalView, GameLobbyStateArguments gameLobbyStateArguments)
  : m_graphicalView(graphicalView)
  , m_gameWindow(*graphicalView.getGameWindow())
  , m_controller(graphicalView.getController())
  , m_gameArguments(std::move(gameLobbyStateArguments))
  , m_callbacks(std::make_shared<GameLobbyCallbacks>(*this))
{
  // Connect GameChoosing callbacks to packet handler
  m_controller.getPacketHandler()->connectCallbacks(m_callbacks.get());
}


void GameLobbyState::init()
{
  LOG(DEBUG) << "[GameLobbyState]::init";
  LOG(DEBUG) << "[GameLobbyState] args: " << m_gameArguments.gameName;
}


void GameLobbyState::eventHandling()
{
  // Process events
  sf::Event event{};
  while (m_gameWindow.pollEvent(event))
  {
    // Handle ImGui events
    ImGui::SFML::ProcessEvent(m_gameWindow, event);

    // Close window: exit
    if (event.type == sf::Event::Closed) {
      m_gameWindow.close();
      m_graphicalView.stop();
      break;
    }
  }
}


void GameLobbyState::display()
{
  ImGui::SFML::Update(m_gameWindow, m_deltaClock.restart());

  // TODO: Test-purpose button only
  if(ImGui::Button("Return to game selection"))
  {
    m_graphicalView.changeState(States::GameChoosing);
  }

  //ImGui::SetCursorPos(ImVec2(-FLT_MIN, 60.f));
  switch(m_lobbyState){
    case LobbyState::Main:
      _guiDisplayMainGui();
      break;
    case LobbyState::JoinLobby:
      _guiDisplayJoinLobby();
      break;
    case LobbyState::CreateLobby:
      _guiDisplayCreateNewLobby();
      break;
    case LobbyState::LobbyDetails:
      _guiDisplayLobby();
      break;
  }

  //m_gameWindow.clear(sf::Color(55, 55, 55));
  m_gameWindow.clear(sf::Color::Magenta);

  ImGui::SFML::Render(m_gameWindow);
  m_gameWindow.display();
}


void GameLobbyState::_guiDisplayMainGui()
{
  if(ImGui::Button("Create Lobby"))
  {
    m_lobbyState = LobbyState::CreateLobby;
  }

  if(ImGui::Button("Join Lobby"))
  {
    m_lobbyState = LobbyState::JoinLobby;
  }
}


void GameLobbyState::_guiDisplayLobby()
{
  // TODO: Change it
  ImGui::BeginTable("Players", 3);
  ImGui::TableSetupColumn("Player");
  ImGui::TableSetupColumn("Something");
  ImGui::TableSetupColumn("Something else");
  ImGui::TableHeadersRow();
  ImGui::TableNextRow();
  ImGui::TableNextColumn();
  ImGui::Text("Example");
  ImGui::TableNextColumn();
  ImGui::Text("Example 2");
  ImGui::TableNextColumn();
  ImGui::Text("Example 3");
  ImGui::TableNextRow();
  ImGui::TableNextColumn();
  ImGui::Text("Example 4");
  ImGui::TableNextColumn();
  ImGui::Text("Example 5");
  ImGui::TableNextColumn();
  ImGui::Text("Example 6");
  ImGui::EndTable();

  // Back button
  if(ImGui::Button("Back"))
  {
    m_lobbyState = LobbyState::Main;
  }
}


void GameLobbyState::_guiDisplayJoinLobby()
{
  // Back button
  if(ImGui::Button("Back"))
  {
    m_lobbyState = LobbyState::Main;
  }
}


void GameLobbyState::_guiDisplayCreateNewLobby()
{
  constexpr size_t MAX_LOBBY_NAME_SIZE = 32;
  static std::string lobbyName;

  ImGui::InputText("Lobby name", &lobbyName);
  if (ImGui::Button("Create lobby"))
  {
    LOG(DEBUG) << lobbyName;
    if (lobbyName.empty()) {
      m_createNewLobbyErrorString = "Lobby name cannot be empty!";
    } else if (lobbyName.size() > MAX_LOBBY_NAME_SIZE) {
      m_createNewLobbyErrorString = "Lobby name too long!";
    } else {
      nlohmann::json createLobbyRequestJson;
      createLobbyRequestJson["LobbyName"] = lobbyName;
      createLobbyRequestJson["GameKey"] = m_gameArguments.gameName;

      m_controller.sendRequest(PacketType::CreateLobby, createLobbyRequestJson.dump());

      nlohmann::json getLobbyDetailsJson;
      getLobbyDetailsJson["ClientID"] =
      m_controller.sendRequest(PacketType::GetLobbyDetails, R"({"ClientID": })");

      m_lobbyState = LobbyState::LobbyDetails;
    }
  }

  ImGui::Text("%s", m_createNewLobbyErrorString.c_str());

  // Back button
  if(ImGui::Button("Back"))
  {
    m_lobbyState = LobbyState::Main;
  }
}

} // namespace
