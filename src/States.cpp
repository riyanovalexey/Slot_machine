#include "States.h"
#include "SlotMachine.h"
#include <iostream>
#include <thread>
#include "GameConstants.h"

void WaitingForActionState::enter() {
    m_slotMachine = getSlotMachine();
    if (!m_slotMachine) return;
    
    m_slotMachine->setStartButtonPressed(false);
    m_slotMachine->setStopButtonPressed(false);
    
    m_slotMachine->getUI().setStartButtonActive(true);
    m_slotMachine->getUI().setStopButtonActive(false);
    m_slotMachine->getUI().setBetButtonsActive(true);
}

void WaitingForActionState::update(float deltaTime) {
    if (!m_slotMachine) return;

    if (m_slotMachine->isStartButtonPressed()) {
        if (m_slotMachine->getPlayerBalance() >= m_slotMachine->getBetAmount()) {
            m_slotMachine->setStartButtonPressed(false);
            triggerEvent(EventType::StartButtonPressed);
        } else {
            m_slotMachine->setStartButtonPressed(false);
        }
    }
}

void WaitingForActionState::exit() {
}

void ReelsSpinningState::enter() {
    m_slotMachine = getSlotMachine();
    if (!m_slotMachine) return;
    
    int currentBet = m_slotMachine->getBetAmount();
    m_slotMachine->setPlayerBalance(m_slotMachine->getPlayerBalance() - currentBet);
    
    m_slotMachine->setStartButtonPressed(false);
    m_slotMachine->setStopButtonPressed(false);
    
    m_slotMachine->getUI().setStartButtonActive(false);
    m_slotMachine->getUI().setStopButtonActive(true);
    m_slotMachine->getUI().setBetButtonsActive(false);
    
    m_slotMachine->getUI().stopWinAnimation();
    
    m_spinTimer = 0.0f;
    
    auto randomResults = m_slotMachine->generateRandomResult();
    m_slotMachine->setReelResults(randomResults);
    
    m_slotMachine->spinReels();
}

void ReelsSpinningState::update(float deltaTime) {
    if (!m_slotMachine) return;

    m_spinTimer += deltaTime;
    
    if (m_slotMachine->areReelsSpinning()) {
        if (m_slotMachine->isStopButtonPressed() || 
            (m_spinTimer >= GameConstants::AUTO_STOP_TIME)) {
            
            m_slotMachine->setStopButtonPressed(false);
            m_slotMachine->getUI().setStopButtonActive(false);
            
            const auto& targetSymbols = m_slotMachine->getReelResults();
            
            m_slotMachine->stopReelsAt(targetSymbols);
        }
    } else {
        if (m_spinTimer > 0.5f) {
            auto actualResults = m_slotMachine->getVisibleReelSymbols();
            
            const auto& targetResults = m_slotMachine->getReelResults();
            
            triggerEvent(EventType::SpinningCompleted);
        }
    }
}

void ReelsSpinningState::exit() {
}

void ShowWinState::enter() {
    m_slotMachine = getSlotMachine();
    if (!m_slotMachine) return;

    const auto& results = m_slotMachine->getReelResults();
    
    int winAmount = m_slotMachine->calculateWin(results, m_slotMachine->getBetAmount());
    m_slotMachine->setWinAmount(winAmount);
    
    if (winAmount > 0) {
        m_slotMachine->setPlayerBalance(m_slotMachine->getPlayerBalance() + winAmount);
        m_winDisplayTime = GameConstants::WIN_DISPLAY_TIME;
    } else {
        m_winDisplayTime = GameConstants::NO_WIN_DISPLAY_TIME;
    }
    
    m_slotMachine->getUI().startWinAnimation(winAmount);
    
    m_slotMachine->setStartButtonPressed(false);
    m_slotMachine->setStopButtonPressed(false);
    m_displayTimer = 0.0f;
    
    m_slotMachine->getUI().setStartButtonActive(false);
    m_slotMachine->getUI().setStopButtonActive(false);
    m_slotMachine->getUI().setBetButtonsActive(false);
}

void ShowWinState::update(float deltaTime) {
    if (!m_slotMachine) return;

    m_displayTimer += deltaTime;
    
    if (m_displayTimer >= m_winDisplayTime) {
        triggerEvent(EventType::WinDisplayCompleted);
    }
}

void ShowWinState::exit() {
    if (!m_slotMachine) return;
    
    m_slotMachine->setWinAmount(0);
    
    m_slotMachine->getUI().stopWinAnimation();
}

