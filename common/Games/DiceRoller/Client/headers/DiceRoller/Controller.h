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

class DiceRollerController final : public Controller
{
public:
  DiceRollerController(sf::TcpSocket& serverSocket, std::atomic_bool& runThreads);
  ~DiceRollerController() final = default;

  void runInBackground() final;
  void viewCallback(std::any& object) final;

private:
  void update();
  void _run();

  network::ClientPacketHandler m_clientPacketHandler;

  DiceRollerViewLogic m_logic;

  std::mutex m_mutex; ///< Mutex for shared resources.
};

} // namespaces
