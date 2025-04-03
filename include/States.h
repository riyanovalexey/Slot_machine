#pragma once

#include "State.h"
#include "StateMachine.h"
#include "SlotMachine.h"
#include <vector>

class WaitingForActionState : public SlotMachineState {
public:
    void enter() override;
    void update(float deltaTime) override;
    void exit() override;
};

class ReelsSpinningState : public SlotMachineState {
public:
    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    float m_spinTimer = 0.0f;
};

class ShowWinState : public SlotMachineState {
public:
    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    float m_displayTimer = 0.0f;
    float m_winDisplayTime = 1.0f;
};