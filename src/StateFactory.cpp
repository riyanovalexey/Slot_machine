#include "StateFactory.h"
#include "States.h"
#include "StateMachine.h"
#include <stdexcept>
#include <iostream>

State* StateFactory::getState(StateType type) {
    auto it = m_states.find(type);
    if (it != m_states.end()) {
        return it->second.get();
    }

    auto newState = createState(type);
    if (!newState) {
        return nullptr;
    }
    
    State* statePtr = newState.get();
    
    if (m_stateMachine) {
        statePtr->setStateMachine(m_stateMachine);
    }
    
    m_states[type] = std::move(newState);
    return statePtr;
}

void StateFactory::setStateMachine(StateMachine* stateMachine) {
    m_stateMachine = stateMachine;
}

void StateFactory::clear() {
    m_states.clear();
    m_stateMachine = nullptr;
}

std::unique_ptr<State> StateFactory::createState(StateType type) {
    switch (type) {
        case StateType::WaitingForAction:
            return std::make_unique<WaitingForActionState>();
            
        case StateType::ReelsSpinning:
            return std::make_unique<ReelsSpinningState>();
            
        case StateType::ShowWin:
            return std::make_unique<ShowWinState>();
            
        default:
            std::cerr << "Error: Unknown state type" << std::endl;
            return nullptr;
    }
} 