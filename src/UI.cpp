#include "UI.h"
#include "GameConstants.h"
#include <sstream>
#include <iostream>
#include <cmath>

UI::UI(sf::RenderWindow& window)
    : m_window(window)
    , m_winAnimationTime(0.f)
    , m_isWinAnimationActive(false)
{
}

UI::~UI() = default;

void UI::init() {
    if (!m_font.openFromFile("fonts/verdana.ttf")) {
        std::cerr << "Error: Could not load font file" << std::endl;
    }

    initText();
    initButtons();

    createButtonText("START");
    createButtonText("STOP");
    createButtonText("+");
    createButtonText("-");
}

void UI::createButtonText(const std::string& text) {
    auto buttonText = std::make_unique<sf::Text>(m_font, text, UIConstants::BUTTON_TEXT_SIZE);
    buttonText->setFillColor(UIConstants::BUTTON_TEXT_COLOR);
    m_buttonTexts[text] = std::move(buttonText);
}

void UI::initText() {
    m_balanceText = std::make_unique<sf::Text>(m_font, "", UIConstants::BALANCE_TEXT_SIZE);
    m_betText = std::make_unique<sf::Text>(m_font, "", UIConstants::BET_TEXT_SIZE);
    m_winText = std::make_unique<sf::Text>(m_font, "", UIConstants::WIN_TEXT_SIZE);
    m_keyboardHintsText = std::make_unique<sf::Text>(m_font, "", UIConstants::HINTS_TEXT_SIZE);

    m_balanceText->setFillColor(UIConstants::BALANCE_TEXT_COLOR);
    m_betText->setFillColor(UIConstants::BET_TEXT_COLOR);
    m_winText->setFillColor(UIConstants::WIN_TEXT_COLOR);
    m_keyboardHintsText->setFillColor(UIConstants::HINTS_TEXT_COLOR);
    
    std::ostringstream hints;
    hints << "Keyboard controls: SPACE - Spin, ENTER - Stop, UP/DOWN - Change bet";
    m_keyboardHintsText->setString(hints.str());
    
    drawKeyboardHints();
}

void UI::initButtons() {
    sf::Vector2u windowSize = m_window.getSize();

    m_spinButton.setSize(UIConstants::SPIN_BUTTON_SIZE);
    m_spinButton.setFillColor(UIConstants::SPIN_BUTTON_COLOR);
    m_spinButton.setPosition(sf::Vector2f(
        windowSize.x - UIConstants::RIGHT_MARGIN - UIConstants::SPIN_BUTTON_SIZE.x,
        windowSize.y - UIConstants::BOTTOM_MARGIN - UIConstants::SPIN_BUTTON_SIZE.y
    ));

    m_stopButton.setSize(UIConstants::STOP_BUTTON_SIZE);
    m_stopButton.setFillColor(UIConstants::STOP_BUTTON_COLOR);
    m_stopButton.setPosition(sf::Vector2f(
        windowSize.x - UIConstants::RIGHT_MARGIN - UIConstants::STOP_BUTTON_SIZE.x,
        windowSize.y - UIConstants::BOTTOM_MARGIN - UIConstants::BUTTON_VERTICAL_SPACING - UIConstants::STOP_BUTTON_SIZE.y
    ));

    m_betIncreaseButton.setSize(UIConstants::BET_BUTTON_SIZE);
    m_betIncreaseButton.setFillColor(UIConstants::BET_INCREASE_COLOR);
    m_betIncreaseButton.setPosition(
        sf::Vector2f(windowSize.x - UIConstants::RIGHT_MARGIN - UIConstants::BET_BUTTONS_RIGHT_MARGIN - UIConstants::SPIN_BUTTON_SIZE.x, 
                    windowSize.y - UIConstants::BET_BUTTONS_BOTTOM_MARGIN - UIConstants::BET_BUTTON_SIZE.y - UIConstants::SPIN_BUTTON_SIZE.y)
    );

    m_betDecreaseButton.setSize(UIConstants::BET_BUTTON_SIZE);
    m_betDecreaseButton.setFillColor(UIConstants::BET_DECREASE_COLOR);
    m_betDecreaseButton.setPosition(
        sf::Vector2f(windowSize.x - UIConstants::RIGHT_MARGIN - UIConstants::BET_BUTTONS_RIGHT_MARGIN - UIConstants::SPIN_BUTTON_SIZE.x, 
                    windowSize.y - UIConstants::BOTTOM_MARGIN - UIConstants::BET_BUTTON_SIZE.y)
    );
}

void UI::draw(int balance, int bet, int winAmount) {
    drawBalance(balance);
    drawBet(bet);
    drawKeyboardHints();
    
    drawButton(m_spinButton, "START", m_startButtonActive);
    drawButton(m_stopButton, "STOP", m_stopButtonActive);
    drawButton(m_betIncreaseButton, "+", m_betButtonsActive);
    drawButton(m_betDecreaseButton, "-", m_betButtonsActive);
    
    m_window.draw(*m_balanceText);
    m_window.draw(*m_betText);
    m_window.draw(*m_keyboardHintsText);
    
    if (m_isWinAnimationActive && m_winText) {
        m_window.draw(*m_winText);
    }
}

void UI::drawBalance(int balance) {
    std::ostringstream ss;
    ss << "Balance: " << balance;
    m_balanceText->setString(ss.str());
    m_balanceText->setPosition(UIConstants::BALANCE_TEXT_POSITION);
}

void UI::drawBet(int bet) {
    std::ostringstream ss;
    ss << "Bet: " << bet;
    m_betText->setString(ss.str());
    m_betText->setPosition(UIConstants::BET_TEXT_POSITION);
}

void UI::startWinAnimation(int winAmount) {
    std::ostringstream ss;
    if (winAmount > 0) {
        ss << "WIN: " << winAmount;
    } else {
        ss << "NO WIN";
    }
    
    m_winText->setString(ss.str());
    
    sf::Vector2u windowSize = m_window.getSize();
    
    m_winText->setPosition(
        sf::Vector2f((windowSize.x - m_winText->getGlobalBounds().size.x) / 2.f,
                    windowSize.y / 2.f)
    );
    
    m_isWinAnimationActive = true;
    m_winAnimationTime = 0.f;
}

bool UI::handleClick(const sf::Vector2i& mousePos) {
    return isStartButtonPressed(mousePos) ||
           isStopButtonPressed(mousePos) ||
           isBetIncreasePressed(mousePos) ||
           isBetDecreasePressed(mousePos);
}

bool UI::isStartButtonPressed(const sf::Vector2i& mousePos) const {
    return m_startButtonActive && m_spinButton.getGlobalBounds().contains(
        sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)));
}

bool UI::isStopButtonPressed(const sf::Vector2i& mousePos) const {
    return m_stopButtonActive && m_stopButton.getGlobalBounds().contains(
        sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)));
}

void UI::update(float deltaTime) {
}

bool UI::isBetIncreasePressed(const sf::Vector2i& mousePos) const {
    return m_betButtonsActive && m_betIncreaseButton.getGlobalBounds().contains(
        sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)));
}

bool UI::isBetDecreasePressed(const sf::Vector2i& mousePos) const {
    return m_betButtonsActive && m_betDecreaseButton.getGlobalBounds().contains(
        sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)));
}

void UI::updateAnimations(float deltaTime) {
    if (m_isWinAnimationActive) {
        m_winAnimationTime += deltaTime;
        
        sf::Color baseColor;
        if (m_winText->getString().find("WIN:") != sf::String::InvalidPos) {
            baseColor = UIConstants::WIN_COLOR;
        } else {
            baseColor = UIConstants::NO_WIN_COLOR;
        }
        
        float alpha = (std::sin(m_winAnimationTime * 5.f) + 1.f) / 2.f;
        baseColor.a = static_cast<uint8_t>(255 * alpha);
        m_winText->setFillColor(baseColor);
    }
}

void UI::handleWindowResize(const sf::Vector2u& newSize) {
    initButtons();
    
    if (m_isWinAnimationActive && m_winText) {
        m_winText->setPosition(
            sf::Vector2f((newSize.x - m_winText->getGlobalBounds().size.x) / 2.f,
                        newSize.y / 2.f)
        );
    }

    drawKeyboardHints();
}

void UI::drawButton(sf::RectangleShape& button, const std::string& text, bool isActive) {
    if (isActive) {
        if (text == "START") {
            button.setFillColor(UIConstants::SPIN_BUTTON_COLOR);
        } else if (text == "STOP") {
            button.setFillColor(UIConstants::STOP_BUTTON_COLOR);
        } else if (text == "+") {
            button.setFillColor(UIConstants::BET_INCREASE_COLOR);
        } else if (text == "-") {
            button.setFillColor(UIConstants::BET_DECREASE_COLOR);
        } else {
            button.setFillColor(UIConstants::ACTIVE_BUTTON_COLOR);
        }
    } else {
        button.setFillColor(UIConstants::INACTIVE_BUTTON_COLOR);
    }

    m_window.draw(button);
    
    auto it = m_buttonTexts.find(text);
    if (it != m_buttonTexts.end()) {
        auto& buttonText = *(it->second);
        
        sf::FloatRect textBounds = buttonText.getLocalBounds();
        buttonText.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.f,
                                          textBounds.position.y + textBounds.size.y / 2.f));
        
        sf::FloatRect buttonBounds = button.getGlobalBounds();
        buttonText.setPosition(sf::Vector2f(buttonBounds.position.x + buttonBounds.size.x / 2.f,
                                            buttonBounds.position.y + buttonBounds.size.y / 2.f));

        m_window.draw(buttonText);
    }
}

void UI::drawKeyboardHints() {
    sf::Vector2u windowSize = m_window.getSize();
    m_keyboardHintsText->setPosition(
        sf::Vector2f(UIConstants::LEFT_MARGIN, 
                    windowSize.y - UIConstants::BOTTOM_MARGIN)
    );
}