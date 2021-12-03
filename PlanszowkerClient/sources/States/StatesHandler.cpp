#include "StatesHandler.h"

using namespace pla::client::states;

void StatesHandler::changeState(StatePtr state){
    popState();
    m_states.push(std::move(state));
}


void StatesHandler::pushState(StatePtr state){
    m_states.push(std::move(state));
}


void StatesHandler::popState(){
    if(!m_states.empty()){
        m_states.pop();
    }
}


IStates* StatesHandler::getTopState(){
    return m_states.top().get();
}
