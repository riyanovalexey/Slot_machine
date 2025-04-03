#include "SlotMachine.h"
#include "GameConstants.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

SlotMachine::SlotMachine()
    : m_playerBalance(GameConstants::DEFAULT_BALANCE)
    , m_betAmount(GameConstants::DEFAULT_BET)
    , m_isRunning(true)
    , m_startButtonPressed(false)
    , m_stopButtonPressed(false)
    , m_winAmount(0)
    , m_reelSet(GameConstants::REEL_COUNT, GameConstants::SYMBOLS_PER_REEL)
    , m_rng(std::random_device{}())
{
    m_reelResults.resize(GameConstants::REEL_COUNT, 0);
    
    sf::VideoMode videoMode(sf::Vector2u(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT));
    m_window = std::make_unique<sf::RenderWindow>(
        videoMode,
        "Slot Machine",
        sf::Style::Default
    );
    m_window->setFramerateLimit(60);

    m_ui = std::make_unique<UI>(*m_window);
}

SlotMachine::~SlotMachine() = default;

void SlotMachine::init() {
    m_reelSet.init();
    
    m_ui->init();
    
    m_stateMachine.setSlotMachine(this);
    m_stateMachine.init(StateType::WaitingForAction);
}

void SlotMachine::run() {
    sf::Clock clock;
    
    while (m_isRunning && m_window->isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        
        processEvents();
        update(deltaTime);
        render(deltaTime);
    }
}

void SlotMachine::processEvents() {
    while (const std::optional event = m_window->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window->close();
        }
        else if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            if (keyEvent->code == sf::Keyboard::Key::Escape) {
                m_isRunning = false;
            }
            else if (keyEvent->code == sf::Keyboard::Key::Space) {
                onStartButtonPressed();
            }
            else if (keyEvent->code == sf::Keyboard::Key::Enter) {
                onStopButtonPressed();
            }
            else if (keyEvent->code == sf::Keyboard::Key::Up) {
                setBetAmount(m_betAmount + GameConstants::BET_INCREMENT);
            }
            else if (keyEvent->code == sf::Keyboard::Key::Down && m_betAmount > GameConstants::MIN_BET) {
                setBetAmount(m_betAmount - GameConstants::BET_INCREMENT);
            }
        }
        else if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseEvent->button == sf::Mouse::Button::Left) {
                sf::Vector2i mousePos(mouseEvent->position);
                
                if (m_ui->isStartButtonPressed(mousePos)) {
                    onStartButtonPressed();
                }
                else if (m_ui->isStopButtonPressed(mousePos)) {
                    onStopButtonPressed();
                }
                else if (m_ui->isBetIncreasePressed(mousePos)) {
                    setBetAmount(m_betAmount + GameConstants::BET_INCREMENT);
                }
                else if (m_ui->isBetDecreasePressed(mousePos) && m_betAmount > GameConstants::MIN_BET) {
                    setBetAmount(m_betAmount - GameConstants::BET_INCREMENT);
                }
            }
        }
        else if (event->is<sf::Event::Resized>()) {
            sf::Vector2u newSize = m_window->getSize();
            
            sf::View view = m_window->getView();
            view.setSize(sf::Vector2f(static_cast<float>(newSize.x), static_cast<float>(newSize.y)));
            view.setCenter(sf::Vector2f(static_cast<float>(newSize.x) / 2.0f, static_cast<float>(newSize.y) / 2.0f));
            m_window->setView(view);
            
            m_ui->handleWindowResize(newSize);
        }
    }
}

void SlotMachine::update(float deltaTime) {
    m_stateMachine.update(deltaTime);
    m_reelSet.update(deltaTime);
}

void SlotMachine::render(float deltaTime) {
    m_window->clear(UIConstants::BACKGROUND_COLOR);
    
    m_ui->updateAnimations(deltaTime);
    
    m_reelSet.draw(*m_window);
    
    m_ui->draw(m_playerBalance, m_betAmount, m_winAmount);
    
    m_window->display();
}

void SlotMachine::spinReels() {
    m_reelSet.spinAll();
}

void SlotMachine::stopReelsAt(const std::vector<int>& targetSymbols) {
    m_reelSet.stopAllAt(targetSymbols);
}

void SlotMachine::onStartButtonPressed() {
    m_startButtonPressed = true;
    m_stopButtonPressed = false;
}

void SlotMachine::onStopButtonPressed() {
    m_stopButtonPressed = true;
}

int SlotMachine::calculateWin(const std::vector<int>& results, int betAmount) const {
    if (results[0] == results[1] && results[1] == results[2]) {
        return betAmount * GameConstants::THREE_OF_A_KIND_MULTIPLIER;
    }
    
    if (results[0] == results[1] || results[1] == results[2] || results[0] == results[2]) {
        return betAmount * GameConstants::TWO_OF_A_KIND_MULTIPLIER;
    }
    
    return 0;
}

std::vector<int> SlotMachine::generateRandomResult() const {
    std::vector<int> results;
    results.resize(GameConstants::REEL_COUNT);
    
    std::uniform_int_distribution<int> distribution(0, GameConstants::SYMBOLS_PER_REEL - 1);
    
    for (int i = 0; i < GameConstants::REEL_COUNT; ++i) {
        results[i] = distribution(m_rng);
    }
    
    return results;
} 