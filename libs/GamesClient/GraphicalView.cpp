#include "GraphicalView.h"

#include <Games/States/GameChoosingState.h>
#include <Games/States/GameState.h>
#include <ErrorHandler/ErrorLogger.h>

#include <SFML/Graphics.hpp>

#include <iostream>

using namespace pla::games;

namespace pla::games_client {

GraphicalView::GraphicalView(Controller& controller, std::atomic_bool& run, const sf::Vector2i& windowDim, const std::string& windowName)
  : m_gameWindow(std::make_unique<GameWindow>(sf::VideoMode(windowDim.x, windowDim.y, 32), windowName))
  , m_controller(controller)
  , m_run(run)
{
  m_gameWindow->setFramerateLimit(60.f);

  m_gameWindow->resetGLStates();
}

GraphicalView::~GraphicalView()
{
  ImGui::SFML::Shutdown(*m_gameWindow);
}


void GraphicalView::init()
{
  if (not ImGui::SFML::Init(*m_gameWindow)) {
    err_handler::ErrorLogger::printError("Could not initiate ImGui/SFML!");
  }
  ImGuiIO& io = ImGui::GetIO();

  // Set INI filename to nullptr to disable imgui's .ini file saving
  io.IniFilename = nullptr;

  // Load fonts
  m_fontManager.loadFonts();

  if (not ImGui::SFML::UpdateFontTexture()) {
    err_handler::ErrorLogger::printError("Could not initiate ImGui/SFML fonts!");
  }

  // Set initial state
  changeState(games::States::GameChoosing);
}


void GraphicalView::run()
{
  while (m_run && m_gameWindow->isOpen()) {
    m_states[0]->eventHandling();
    m_states[0]->display();
  }
}


void GraphicalView::changeState(States newState)
{
  std::shared_ptr<IState> newStatePtr;

  switch (newState)
  {
    case States::GameChoosing:
      newStatePtr = std::make_shared<GameChoosingState>(*this);
      break;
    case States::Game:
      newStatePtr = std::make_shared<GameState>(*this);
      break;
    case States::Lobby:
      break;
  }

  m_states.emplace_back(std::move(newStatePtr));

  if (m_states.size() > 1) {
    m_states.pop_front();
  }

  // Run init method from new state
  m_states[0]->init();
}

} // namespaces
