#pragma once
#include <vector>

enum class StateType {
    WaitingForAction,
    ReelsSpinning,
    ShowWin
};

enum class EventType {
    StartButtonPressed,
    SpinningCompleted,
    WinDisplayCompleted
};

struct Transition {
    StateType from;
    EventType event;
    StateType to;
};

namespace GameTransitions {
    inline const std::vector<Transition> TRANSITIONS = {
        {StateType::WaitingForAction, EventType::StartButtonPressed, StateType::ReelsSpinning},
        {StateType::ReelsSpinning, EventType::SpinningCompleted, StateType::ShowWin},
        {StateType::ShowWin, EventType::WinDisplayCompleted, StateType::WaitingForAction}
    };
} 