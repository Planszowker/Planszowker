#include "GraphicalView.h"

#include <Games/States/GameChoosingState.h>
#include <Games/States/GameState.h>

#include <iostream>

using namespace pla::games;

namespace pla::games_client {

GraphicalView::GraphicalView(Controller& controller, std::atomic_bool& run, const sf::Vector2i& windowDim, const std::string& windowName)
  : m_gameWindow(std::make_unique<GameWindow>(sf::VideoMode(windowDim.x, windowDim.y, 32), windowName))
  , m_controller(controller)
  , m_run(run)
{
  m_gameWindow->resetGLStates(); // Reset for SFGUI, since it uses OpenGL for rendering

  // Todo: maybe this hard-coded FPS limit is not a good idea?
  m_gameWindow->setFramerateLimit(60);

  // Set initial state
  changeState(games::States::GameChoosing);
}


void GraphicalView::init()
{
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


void GraphicalView::_recalculateActionBoxWindowSize()
{
//  sf::Vector2f pos = m_actionsAreaWindow->GetAbsolutePosition();
//  std::cout << "Pozycja: " << pos.y << "\n";
//  std::cout << "Req: " << m_actionsAreaWindow->GetAllocation().width << " " << m_actionsAreaWindow->GetAllocation().height << "\n";
//  std::cout << "GameArea: " << m_gameAreaWindow->GetRequisition().x << " " << m_gameAreaWindow->GetRequisition().y << "\n";
//  m_actionsAreaWindow->SetPosition(sf::Vector2f((m_gameAreaWindow->GetRequisition().x - m_actionsAreaWindow->GetRequisition().x) / 2.f, pos.y));
}

} // namespaces
