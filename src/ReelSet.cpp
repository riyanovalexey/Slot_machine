#include "ReelSet.h"
#include <stdexcept>
#include <iostream>
#include "UI.h"
#include "GameConstants.h"
ReelSet::ReelSet(int reelCount, int symbolsPerReel)
    : m_symbolsPerReel(symbolsPerReel)
    , m_reelSpacing(ReelConstants::DEFAULT_REEL_SPACING)
    , m_currentStoppingReel(-1)
    , m_isStoppingSequence(false)
    , m_stopTimer(0.0f)
    , m_targetSymbols()
{
    m_reels.reserve(reelCount);
    for (int i = 0; i < reelCount; ++i) {
        m_reels.push_back(std::make_unique<Reel>(symbolsPerReel));
    }

    m_topMask.setFillColor(UIConstants::BACKGROUND_COLOR);
    m_bottomMask.setFillColor(UIConstants::BACKGROUND_COLOR);
}

ReelSet::~ReelSet() = default;

void ReelSet::init() {
    initReels();
}

void ReelSet::updateMasks(const sf::Vector2u& windowSize) {
    float maskWidth = m_reels.size() * m_reelSpacing;
    float maskHeight = ReelConstants::REELS_HEIGHT;
    float centerY = windowSize.y / 2.0f;

    m_topMask.setSize(sf::Vector2f(maskWidth, centerY - maskHeight));
    m_topMask.setPosition(sf::Vector2f((windowSize.x - maskWidth) / 2.0f, 0.0f));

    m_bottomMask.setSize(sf::Vector2f(maskWidth, centerY - maskHeight));
    m_bottomMask.setPosition(sf::Vector2f((windowSize.x - maskWidth) / 2.0f, centerY + maskHeight));
}

void ReelSet::spinAll() {
    m_isStoppingSequence = false;
    m_currentStoppingReel = -1;
    m_stopTimer = 0.0f;
    
    for (auto& reel : m_reels) {
        reel->spin();
    }
    
    for (size_t i = 0; i < m_reels.size(); ++i) {
        float speedVariation = ReelConstants::SPEED_VARIATION * (i + 1);
        m_reels[i]->setSpinSpeed(ReelConstants::BASE_SPIN_SPEED + speedVariation);
    }
}

void ReelSet::stopAllAt(const std::vector<int>& targetSymbols) {    
    bool anySpinning = isAnySpinning();
    
    if (!m_isStoppingSequence && anySpinning) {
        m_targetSymbols = targetSymbols;
        
        m_isStoppingSequence = true;
        m_currentStoppingReel = 0;
        m_stopTimer = 0.0f;
        
        startNextReelStopAt();
    } else if (!anySpinning) {
        if (m_stopCallback) {
            m_stopCallback(getVisibleSymbols());
        }
    }
}

void ReelSet::startNextReelStop() {
    if (m_currentStoppingReel >= 0 && m_currentStoppingReel < m_reels.size()) {
        m_reels[m_currentStoppingReel]->stop();
    }
}

void ReelSet::startNextReelStopAt() {
    if (m_currentStoppingReel >= 0 && m_currentStoppingReel < m_reels.size()) {
        m_reels[m_currentStoppingReel]->stopAt(m_targetSymbols[m_currentStoppingReel]);
    }
}

void ReelSet::update(float deltaTime) {
    bool allStopped = true;
    
    for (auto& reel : m_reels) {
        reel->update(deltaTime);
        if (reel->isSpinning()) {
            allStopped = false;
        }
    }

    if (m_isStoppingSequence) {
        if (m_currentStoppingReel < m_reels.size()) {
            if (!m_reels[m_currentStoppingReel]->isSpinning()) {
                m_stopTimer += deltaTime;
                if (m_stopTimer >= ReelConstants::STOP_DELAY) {
                    m_currentStoppingReel++;
                    m_stopTimer = 0.0f;
                    if (m_currentStoppingReel < m_reels.size()) {
                        if (!m_targetSymbols.empty()) {
                            startNextReelStopAt();
                        } else {
                            startNextReelStop();
                        }
                    }
                }
            }
        }
        else if (allStopped && m_stopCallback) {
            m_isStoppingSequence = false;
            auto results = getVisibleSymbols();
            
            m_stopCallback(results);
            
            m_targetSymbols.clear();
        }
    }
}

void ReelSet::draw(sf::RenderWindow& window) {
    positionReels(window.getSize());
    updateMasks(window.getSize());

    for (auto& reel : m_reels) {
        reel->draw(window);
    }

    window.draw(m_topMask);
    window.draw(m_bottomMask);
}

bool ReelSet::isAnySpinning() const {
    for (const auto& reel : m_reels) {
        if (reel->isSpinning()) {
            return true;
        }
    }
    
    return false;
}

std::vector<int> ReelSet::getVisibleSymbols() const {
    std::vector<int> symbols;
    symbols.reserve(m_reels.size());
    
    for (const auto& reel : m_reels) {
        symbols.push_back(reel->getCurrentSymbol());
    }
    
    return symbols;
}

Reel& ReelSet::getReel(int index) {
    if (index < 0 || index >= static_cast<int>(m_reels.size())) {
        throw std::out_of_range("Reel index out of range");
    }
    return *m_reels[index];
}

const Reel& ReelSet::getReel(int index) const {
    if (index < 0 || index >= static_cast<int>(m_reels.size())) {
        throw std::out_of_range("Reel index out of range");
    }
    return *m_reels[index];
}

void ReelSet::initReels() {
    for (auto& reel : m_reels) {
        std::vector<std::unique_ptr<Symbol>> symbols;
        for (int i = 0; i < m_symbolsPerReel; ++i) {
            symbols.push_back(std::make_unique<Symbol>(
                i,
                static_cast<Symbol::Type>(i % 5),
                sf::Color(
                    SymbolConstants::COLOR_RED_BASE + i * SymbolConstants::COLOR_RED_INCREMENT, 
                    SymbolConstants::COLOR_GREEN_BASE + i * SymbolConstants::COLOR_GREEN_INCREMENT, 
                    SymbolConstants::COLOR_BLUE_BASE + i * SymbolConstants::COLOR_BLUE_INCREMENT
                )
            ));
        }
        reel->initSymbols(std::move(symbols));
    }
}

void ReelSet::positionReels(const sf::Vector2u& windowSize) {
    float scaleFactor = windowSize.x / ReelConstants::REEL_SPACING_SCALE_BASE;
    m_reelSpacing = ReelConstants::DEFAULT_REEL_SPACING * scaleFactor;
    
    float startX = (windowSize.x - (m_reels.size() * m_reelSpacing)) / 2.f + m_reelSpacing / 2.f;
    
    float centerY = windowSize.y / 2.f;
    
    for (size_t i = 0; i < m_reels.size(); ++i) {
        m_reels[i]->setXPosition(startX + i * m_reelSpacing);
        m_reels[i]->setYPosition(centerY);
    }
} 