#pragma once
#include "GameTypes.h"

class StateMachine;
enum class EventType;
class SlotMachine;

class State {
public:
    virtual ~State() = default;
    
    virtual void enter() = 0;
    
    virtual void update(float deltaTime) = 0;
    
    virtual void exit() = 0;
    
    void setStateMachine(StateMachine* stateMachine) {
        m_stateMachine = stateMachine;
    }
    
protected:
    void triggerEvent(EventType event);
    
    StateMachine* m_stateMachine = nullptr;
};

class SlotMachineState : public State {
public:
    SlotMachine* getSlotMachine() const;
    
protected:
    SlotMachine* m_slotMachine = nullptr;
}; 