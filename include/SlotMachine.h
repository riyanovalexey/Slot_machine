#pragma once

#include "StateMachine.h"
#include "ReelSet.h"
#include "UI.h"
#include <string>
#include <vector>
#include <memory>

class WaitingForActionState;
class ReelsSpinningState;
class ShowWinState;

class SlotMachine {
public:
    SlotMachine();
    ~SlotMachine();

    void init();
    void run();

    int getPlayerBalance() const { return m_playerBalance; }
    void setPlayerBalance(int balance) { m_playerBalance = balance; }

    int getBetAmount() const { return m_betAmount; }
    void setBetAmount(int amount) { m_betAmount = amount; }

    bool isStartButtonPressed() const { return m_startButtonPressed; }
    void setStartButtonPressed(bool pressed) { m_startButtonPressed = pressed; }
    bool isStopButtonPressed() const { return m_stopButtonPressed; }
    void setStopButtonPressed(bool pressed) { m_stopButtonPressed = pressed; }

    const std::vector<int>& getReelResults() const { return m_reelResults; }
    void setReelResults(const std::vector<int>& results) { m_reelResults = results; }

    int getWinAmount() const { return m_winAmount; }
    void setWinAmount(int amount) { m_winAmount = amount; }

    bool isRunning() const { return m_isRunning; }
    void setRunning(bool running) { m_isRunning = running; }

    void spinReels();
    void stopReels();
    
    std::vector<int> generateRandomResult() const;
    void stopReelsAt(const std::vector<int>& targetSymbols);
    
    bool areReelsSpinning() const { return m_reelSet.isAnySpinning(); }
    std::vector<int> getVisibleReelSymbols() const { return m_reelSet.getVisibleSymbols(); }
    
    int calculateWin(const std::vector<int>& results, int betAmount) const;

    void update(float deltaTime);
    void processEvents();

    void onStartButtonPressed();
    void onStopButtonPressed();

    sf::RenderWindow& getWindow() { return *m_window; }
    UI& getUI() { return *m_ui; }
    void render(float deltaTime);

private:
    StateMachine m_stateMachine;
    
    std::unique_ptr<sf::RenderWindow> m_window;
    ReelSet m_reelSet;
    std::unique_ptr<UI> m_ui;
    
    int m_playerBalance;
    int m_betAmount;
    bool m_isRunning;

    bool m_startButtonPressed;
    bool m_stopButtonPressed;
    std::vector<int> m_reelResults;
    int m_winAmount;
}; 