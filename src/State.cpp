#include "State.h"
#include "StateMachine.h"
#include "SlotMachine.h"

void State::triggerEvent(EventType event) {
    if (m_stateMachine) {
        m_stateMachine->processEvent(event);
    }
}

SlotMachine* SlotMachineState::getSlotMachine() const {
    if (m_stateMachine) {
        return m_stateMachine->getSlotMachine();
    }
    return nullptr;
} 