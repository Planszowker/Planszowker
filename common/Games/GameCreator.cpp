#include "GameCreator.h"

namespace pla::common::games {

GameCreator::GameCreator(const GamesList &game)
  : m_game(game)
  , m_running(false)
{
}


void GameCreator::runClient()
{
  m_running = true;
}

} // namespaces
