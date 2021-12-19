#include "Client.h"

#include "GraphicalView.h"
#include "Controller.h"

namespace pla::common::games::dice_roller {

DiceRollerClient::DiceRollerClient(sf::TcpSocket& serverSocket)
  : m_view(std::make_unique<DiceRollerGraphicalView>(sf::Vector2i(1280, 720), "DiceRoller"))
  , m_controller(std::make_unique<DiceRollerController>(m_serverSocket, m_runThreads))
  , m_serverSocket(serverSocket)
{
  // Connect view and controller
  m_controller->connectView(m_view.get());
  m_view->connectController(m_controller.get());

  // Initialize controller and view
  m_controller->init();
  m_view->init();

  // Run controller in separate thread
  m_controller->runInBackground();

  // Run view
  m_view->run();

  // Stop other threads if view has ended
  m_runThreads = false;
}

} // namespaces
