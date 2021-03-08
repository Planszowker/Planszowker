#pragma once

#include "ViewLogic.h"
#include "ConsoleView.h"

#include <SFML/Network.hpp>

#include <any>
#include <atomic>
#include <mutex>

namespace pla::common::games {

class GenericView;

class Controller
{
public:
  explicit Controller(sf::TcpSocket& serverSocket)
    : m_serverSocket(serverSocket)
    , m_runController(true)
  {
  }

  virtual void run() = 0;
  virtual void viewCallback(std::any object) = 0;
  virtual void receiveThread(std::mutex& mutex) = 0;

  std::unique_ptr<ViewLogic>& getViewLogic() { return m_logic; }

protected:
  virtual void update() = 0;

  std::unique_ptr<ViewLogic> m_logic;
  sf::TcpSocket& m_serverSocket;
  std::atomic_bool m_runController;

  std::mutex m_mutex;
};

} // namespaces
