#pragma once

#include "State.h"
#include "StateMachine.h"
#include "SlotMachine.h"
#include <vector>

class WaitingForActionState : public State {
public:
    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    SlotMachine* m_slotMachine = nullptr;
};

class ReelsSpinningState : public State {
public:
    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    float m_spinTimer = 0.0f;
    SlotMachine* m_slotMachine = nullptr;
};

class ShowWinState : public State {
public:
    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    float m_displayTimer = 0.0f;
    float m_winDisplayTime = 1.0f;
    SlotMachine* m_slotMachine = nullptr;
};