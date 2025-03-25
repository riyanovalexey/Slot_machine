#pragma once

#include "Reel.h"
#include <vector>
#include <memory>
#include <functional>

class ReelSet {
public:
    using StopCallback = std::function<void(const std::vector<int>&)>;

    ReelSet(int reelCount, int symbolsPerReel);
    ~ReelSet();

    void init();
    void spinAll();
    void stopAllAt(const std::vector<int>& targetSymbols);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

    bool isAnySpinning() const;
    std::vector<int> getVisibleSymbols() const;

    Reel& getReel(int index);
    const Reel& getReel(int index) const;

    void setStopCallback(StopCallback callback) { m_stopCallback = callback; }

private:
    void initReels();
    void positionReels(const sf::Vector2u& windowSize);
    void startNextReelStop();
    void startNextReelStopAt();
    void updateMasks(const sf::Vector2u& windowSize);

    std::vector<std::unique_ptr<Reel>> m_reels;
    int m_symbolsPerReel;
    float m_reelSpacing;
    int m_currentStoppingReel;
    bool m_isStoppingSequence;
    StopCallback m_stopCallback;
    float m_stopTimer;
    std::vector<int> m_targetSymbols;

    sf::RectangleShape m_topMask;
    sf::RectangleShape m_bottomMask;
}; 