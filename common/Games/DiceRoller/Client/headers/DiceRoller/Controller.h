#pragma once

/* Generic */
#include "Games/Controller.h"
#include "NetworkHandler/ClientPacketHandler.h"

/* DiceRoller specific */
#include "GraphicalView.h"
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
  DiceRollerController(sf::TcpSocket& serverSocket, std::atomic_bool& runThreads);

  void run() final;
  void viewCallback(std::any& object) final;

private:
  void update();

  network::ClientPacketHandler m_clientPacketHandler;

  DiceRollerViewLogic m_logic;

  std::atomic_bool& m_runThreads;
};

} // namespaces
