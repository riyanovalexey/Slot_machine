#pragma once

#include "GameTypes.h"
#include <memory>
#include <unordered_map>

class State;
class StateMachine;

class StateFactory {
public:
    State* getState(StateType type);
    
    void setStateMachine(StateMachine* stateMachine);
    
    void clear();

private:
    std::unique_ptr<State> createState(StateType type);
    
    std::unordered_map<StateType, std::unique_ptr<State>> m_states;
    
    StateMachine* m_stateMachine = nullptr;
}; 