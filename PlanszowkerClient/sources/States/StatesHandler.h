#ifndef STATES_HANDLER_H
#define STATES_HANDLER_H

#include "IStates.h"

#include <stack>
#include <memory>

namespace pla::client::states
{

typedef std::unique_ptr<IStates> StatePtr;

class StatesHandler{
public:
  StatesHandler() { }
  ~StatesHandler() { }

  void changeState(StatePtr state);
  void pushState(StatePtr state);
  void popState();

  IStates* getTopState();

  bool isEmpty() { return m_states.empty(); }
private:
    std::stack<StatePtr> m_states; // LIFO structure
};

} // namespace

#endif // STATES_HANDLER_H
