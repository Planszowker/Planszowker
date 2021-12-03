#pragma once

/* Generic */
#include "Common.h"

/* STD */
#include <atomic>

namespace pla::common::games {

class GameCreator
{
public:
  GameCreator(const GamesList& game);

  void runClient();
  void runServer();

  void stop();

private:
  GamesList m_game;
  std::atomic_bool m_running;
};

} // namespaces
