#pragma once

/* Generic */
#include "Games/Controller.h"
#include "Games/GenericView.h"
#include "GameWindow/GameWindow.h"

/* SFML */
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

/* STD */
#include <atomic>

namespace pla::common::games::dice_roller {

class DiceRollerClient // todo: inherit from generic class Client to be able to switch between games
{
public:
  explicit DiceRollerClient(sf::TcpSocket& serverSocket);

private:
  sf::TcpSocket& m_serverSocket;
  std::atomic_bool m_runThreads = true;

  std::unique_ptr<GenericView> m_view;
  std::unique_ptr<Controller> m_controller;
};

} // namespaces
