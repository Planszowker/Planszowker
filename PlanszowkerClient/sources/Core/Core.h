#ifndef CORE_H
#define CORE_H

#include <SFML/Graphics.hpp>

#include "States/StatesHandler.h"

namespace pla::client::core
{

class Core {
public:
  Core();

  void runMainLoop();

  states::StatesHandler& GetStates();

private:
  states::StatesHandler m_stateHandler;
};

} // namespace

#endif // CORE_H