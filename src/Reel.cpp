#include "Reel.h"
#include "GameConstants.h"
#include <iostream>
#include <ReelSet.h>
#include "SlotMachine.h"
#include <cmath>

Reel::Reel(int symbolCount)
    : m_currentPosition(0.f)
    , m_xPosition(0.f)
    , m_yPosition(0.f)
    , m_spinSpeed(0.f)
    , m_targetSpeed(ReelConstants::MAX_SPIN_SPEED)
    , m_isSpinning(false)
    , m_isAccelerating(false)
    , m_currentSymbol(0)
    , m_targetSymbol(-1)
    , m_symbolHeight(ReelConstants::SYMBOL_HEIGHT)
    , m_deceleration(0.0f)
{
}

Reel::~Reel() = default;

void Reel::initSymbols(std::vector<std::unique_ptr<Symbol>> symbols) {
    m_symbols = std::move(symbols);
    
    for (size_t i = 0; i < m_symbols.size(); ++i) {
        float yPos = i * m_symbolHeight;
        m_symbols[i]->setPosition(sf::Vector2f(m_xPosition, yPos));
        m_symbols[i]->setScale(sf::Vector2f(1.f, 1.f));
    }
}

void Reel::spin() {
    m_isSpinning = true;
    m_isAccelerating = true;
    m_targetSymbol = -1;
    m_deceleration = 0.0f;
    
    m_spinSpeed = ReelConstants::MIN_SPIN_SPEED;
    m_targetSpeed = ReelConstants::MAX_SPIN_SPEED;
}

void Reel::stop() {
    if (!m_isSpinning) {
        return;
    }
    
    m_isAccelerating = false;
    
    calculateTargetSymbol();
    
    calculateDecelerationToTarget();
}

void Reel::stopAt(int targetSymbolIndex) {
    if (!m_isSpinning) {
        return;
    }
    
    m_isAccelerating = false;
    
    if (targetSymbolIndex < 0 || targetSymbolIndex >= m_symbols.size()) {
        calculateTargetSymbol();
    } else {
        m_targetSymbol = targetSymbolIndex;
    }
    
    calculateDecelerationToTarget();
}

void Reel::calculateDecelerationToTarget() {
    float totalHeight = m_symbols.size() * m_symbolHeight;
    float currentPosition = m_currentPosition;
    while (currentPosition >= totalHeight) {
        currentPosition -= totalHeight;
    }
    
    float targetPosition = m_targetSymbol * m_symbolHeight;
    
    int minRotations = ReelConstants::MIN_ROTATIONS_BASE + 
                       (m_spinSpeed > ReelConstants::HIGH_SPEED_THRESHOLD ? 
                        ReelConstants::ADDITIONAL_ROTATION_FOR_HIGH_SPEED : 0);
    float baseDistance = minRotations * totalHeight;
    
    float directDistanceToTarget = targetPosition - currentPosition;
    if (directDistanceToTarget <= 0) {
        directDistanceToTarget += totalHeight;
    }
    
    float totalDistance = baseDistance + directDistanceToTarget;
    
    // S = V²/2a, a = V²/2S
    m_deceleration = (m_spinSpeed * m_spinSpeed) / (2.0f * totalDistance);
    
    float maxDeceleration = m_spinSpeed / ReelConstants::MAX_DECELERATION_DIVIDER;
    m_deceleration = std::min(m_deceleration, maxDeceleration);
}

void Reel::calculateTargetSymbol() {
    float totalHeight = m_symbols.size() * m_symbolHeight;
    float normalizedPosition = m_currentPosition;
    while (normalizedPosition >= totalHeight) {
        normalizedPosition -= totalHeight;
    }
    
    m_targetSymbol = static_cast<int>(std::round(normalizedPosition / m_symbolHeight)) % m_symbols.size();
}

void Reel::update(float deltaTime) {
    if (!m_isSpinning) return;

    float oldSpeed = m_spinSpeed;
    
    if (m_isAccelerating) {
        m_spinSpeed = std::min(m_targetSpeed, m_spinSpeed + ReelConstants::ACCELERATION_RATE * deltaTime);
        
        if (m_spinSpeed >= m_targetSpeed) {
            m_isAccelerating = false;
            m_spinSpeed = m_targetSpeed;
        }
    } 
    else if (m_targetSymbol >= 0) {
        m_spinSpeed = std::max(0.0f, m_spinSpeed - m_deceleration * deltaTime);
        
        if (m_spinSpeed < GameConstants::MIN_SPIN_SPEED_THRESHOLD) {
            m_isSpinning = false;
            m_spinSpeed = 0.0f;
            m_currentSymbol = m_targetSymbol;
            m_currentPosition = m_targetSymbol * m_symbolHeight;
            
            return;
        }
    }
    
    float avgSpeed = (oldSpeed + m_spinSpeed) / 2.0f;
    m_currentPosition += avgSpeed * deltaTime;
    
    float totalHeight = m_symbols.size() * m_symbolHeight;
    while (m_currentPosition >= totalHeight) {
        m_currentPosition -= totalHeight;
    }
    
    m_currentSymbol = static_cast<int>(m_currentPosition / m_symbolHeight);
}

void Reel::draw(sf::RenderWindow& window) {
    float centerY = m_yPosition != 0.0f ? m_yPosition : window.getSize().y / 2.0f;
    
    float yOffset = m_currentPosition - (m_currentSymbol * m_symbolHeight);
    
    for (int i = -(GameConstants::VISIBLE_SYMBOLS / 2 + 1); i <= GameConstants::VISIBLE_SYMBOLS / 2 + 1; ++i) {
        int symbolIndex = m_currentSymbol + i;
        if (symbolIndex < 0) symbolIndex += m_symbols.size();
        if (symbolIndex >= m_symbols.size()) symbolIndex -= m_symbols.size();
        
        float y = centerY + (m_symbolHeight * i - yOffset);
        
        float symbolTop = y - m_symbolHeight / 2.0f;
        float symbolBottom = y + m_symbolHeight / 2.0f;
        
        if (symbolTop < centerY + ReelConstants::REELS_HEIGHT && 
            symbolBottom > centerY - ReelConstants::REELS_HEIGHT) {
            if (m_symbols[symbolIndex]) {
                m_symbols[symbolIndex]->setPosition(sf::Vector2f(m_xPosition, y));
                m_symbols[symbolIndex]->draw(window);
            }
        }
    }
}

float Reel::getCurrentPosition() const {
    return m_currentPosition;
}

float Reel::getSymbolHeight() const {
    return m_symbolHeight;
} 