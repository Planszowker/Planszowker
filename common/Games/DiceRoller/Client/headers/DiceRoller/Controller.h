#pragma once

#include "Games/Controller.h"
#include "Games/ViewLogic.h"
#include "ConsoleView.h"

#include "SFML/Network.hpp"

#include <memory>
#include <any>
#include <atomic>

namespace pla::common::games::dice_roller {

class DiceRollerController : public Controller
{
public:
  explicit DiceRollerController(sf::TcpSocket& serverSocket);

  void run() final;
  void viewCallback(std::any object) final;
  void receiveThread(std::mutex& mutex) final;

private:
  void update() final;
};

} // namespace
