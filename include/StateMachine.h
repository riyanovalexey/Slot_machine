#pragma once

#include "GameTypes.h"
#include "StateFactory.h"
#include <memory>
#include <unordered_map>
#include <vector>

class State;
class SlotMachine;

class StateMachine {
public:
    StateMachine();
    ~StateMachine();

    void init(StateType initialState);

    void update(float deltaTime);

    void processEvent(EventType event);

    StateType getCurrentStateType() const { return m_currentStateType; }

    void setSlotMachine(SlotMachine* slotMachine) { m_slotMachine = slotMachine; }

    SlotMachine* getSlotMachine() const { return m_slotMachine; }

private:
    void initTransitionTable();
    
    void registerTransition(StateType fromState, EventType event, StateType toState);

    const char* getStateTypeName(StateType type) const;
    const char* getEventTypeName(EventType event) const;

    State* m_currentState = nullptr;
    
    StateType m_currentStateType;
    
    StateFactory m_stateFactory;
    
    SlotMachine* m_slotMachine = nullptr;
    
    std::unordered_map<int, StateType> m_transitions;
    
    int getTransitionKey(StateType state, EventType event) const {
        return static_cast<int>(state) * 100 + static_cast<int>(event);
    }
    
    void changeState(StateType newStateType);
}; 