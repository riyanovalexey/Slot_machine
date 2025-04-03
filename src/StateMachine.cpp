#include "StateMachine.h"
#include "State.h"
#include "States.h"
#include "StateFactory.h"
#include <iostream>
#include <stdexcept>

StateMachine::StateMachine() : m_currentState(nullptr), m_currentStateType(StateType::WaitingForAction) {
    m_stateFactory.setStateMachine(this);
}

StateMachine::~StateMachine() {
    if (m_currentState) {
        m_currentState->exit();
    }
}

void StateMachine::init(StateType initialState) {
    initTransitionTable();
    changeState(initialState);
}

void StateMachine::initTransitionTable() {
    for (const auto& transition : GameTransitions::TRANSITIONS) {
        registerTransition(transition.from, transition.event, transition.to);
    }
}

void StateMachine::registerTransition(StateType fromState, EventType event, StateType toState) {
    int key = getTransitionKey(fromState, event);
    m_transitions[key] = toState;
}

void StateMachine::update(float deltaTime) {
    if (m_currentState) {
        m_currentState->update(deltaTime);
    }
}

void StateMachine::processEvent(EventType event) {
    if (!m_currentState) {
        std::cerr << "Error: No current state set" << std::endl;
        return;
    }

    int key = getTransitionKey(m_currentStateType, event);
    auto it = m_transitions.find(key);
    
    if (it == m_transitions.end()) {
        return;
    }

    changeState(it->second);
}

void StateMachine::changeState(StateType newStateType) {
    if (m_currentState) {
        m_currentState->exit();
    }

    m_currentStateType = newStateType;
    m_currentState = m_stateFactory.getState(newStateType);

    if (!m_currentState) {
        std::cerr << "Error: Failed to create new state" << std::endl;
        m_currentStateType = StateType::WaitingForAction;
        m_currentState = m_stateFactory.getState(m_currentStateType);
        
        if (!m_currentState) {
            std::cerr << "Critical Error: Cannot recover to safe state" << std::endl;
        }
    } else {
        m_currentState->enter();
    }
}

const char* StateMachine::getStateTypeName(StateType type) const {
    switch (type) {
        case StateType::WaitingForAction: return "WaitingForAction";
        case StateType::ReelsSpinning: return "ReelsSpinning";
        case StateType::ShowWin: return "ShowWin";
        default: return "Unknown";
    }
}

const char* StateMachine::getEventTypeName(EventType event) const {
    switch (event) {
        case EventType::StartButtonPressed: return "StartButtonPressed";
        case EventType::SpinningCompleted: return "SpinningCompleted";
        case EventType::WinDisplayCompleted: return "WinDisplayCompleted";
        default: return "Unknown";
    }
} 