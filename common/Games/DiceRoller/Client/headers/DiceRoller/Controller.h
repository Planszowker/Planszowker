#pragma once

/* Generic */
#include "Games/Controller.h"
#include "NetworkHandler/ClientPacketHandler.h"

/* DiceRoller specific */
#include "ConsoleView.h"
#include "ViewLogic.h"

/* SFML */
#include "SFML/Network.hpp"

/* STD */
#include <memory>
#include <any>
#include <atomic>

namespace pla::common::games::dice_roller {

class DiceRollerController : public Controller
{
public:
  explicit DiceRollerController(sf::TcpSocket& serverSocket);

  void run() final;
  void viewCallback(std::any& object) final;

private:
  void update();

  network::ClientPacketHandler m_clientPacketHandler;

  DiceRollerConsoleView m_view;
  DiceRollerViewLogic m_logic;
};

} // namespaces
