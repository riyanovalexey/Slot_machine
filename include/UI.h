#pragma once

#include <SFML/Graphics.hpp>
#include "ReelSet.h"
#include <map>

class UI {
public:
    UI(sf::RenderWindow& window);
    ~UI();

    void init();
    void draw(int balance, int bet, int winAmount);
    
    void startWinAnimation(int winAmount);
    void stopWinAnimation() { m_isWinAnimationActive = false; }
    bool isWinAnimationActive() const { return m_isWinAnimationActive; }

    bool handleClick(const sf::Vector2i& mousePos);
    void update(float deltaTime);
    void updateAnimations(float deltaTime);

    bool isStartButtonPressed(const sf::Vector2i& mousePos) const;
    bool isStopButtonPressed(const sf::Vector2i& mousePos) const;
    bool isBetIncreasePressed(const sf::Vector2i& mousePos) const;
    bool isBetDecreasePressed(const sf::Vector2i& mousePos) const;

    void setStartButtonActive(bool active) { m_startButtonActive = active; }
    void setStopButtonActive(bool active) { m_stopButtonActive = active; }
    void setBetButtonsActive(bool active) { m_betButtonsActive = active; }
    
    void handleWindowResize(const sf::Vector2u& newSize);

private:
    void initButtons();
    void initText();
    void drawButton(sf::RectangleShape& button, const std::string& text, bool isActive = true);
    void createButtonText(const std::string& text);
    
    void drawBalance(int balance);
    void drawBet(int bet);
    void drawKeyboardHints();

    sf::RenderWindow& m_window;

    sf::RectangleShape m_spinButton;
    sf::RectangleShape m_stopButton;
    sf::RectangleShape m_betIncreaseButton;
    sf::RectangleShape m_betDecreaseButton;

    sf::Font m_font;
    std::unique_ptr<sf::Text> m_balanceText;
    std::unique_ptr<sf::Text> m_betText;
    std::unique_ptr<sf::Text> m_winText;
    std::unique_ptr<sf::Text> m_keyboardHintsText;
    std::map<std::string, std::unique_ptr<sf::Text>> m_buttonTexts;

    float m_winAnimationTime;
    bool m_isWinAnimationActive;

    bool m_startButtonActive = true;
    bool m_stopButtonActive = false;
    bool m_betButtonsActive = true;
}; 