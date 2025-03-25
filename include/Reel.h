#pragma once

#include "Symbol.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Reel {
public:
    Reel(int symbolCount);
    ~Reel();

    void initSymbols(std::vector<std::unique_ptr<Symbol>> symbols);

    void spin();
    void stop();
    void stopAt(int targetSymbolIndex);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

    bool isSpinning() const { return m_isSpinning; }
    void setSpinSpeed(float speed) { m_targetSpeed = speed; }
    float getCurrentPosition() const;
    float getSymbolHeight() const;

    int getCurrentSymbol() const { return m_currentSymbol; }
    float getPosition() const { return m_currentPosition; }

    void setPosition(float pos) { m_currentPosition = pos; }
    void setXPosition(float x) { m_xPosition = x; }
    void setYPosition(float y) { m_yPosition = y; }

private:
    void calculateTargetSymbol();
    void calculateDecelerationToTarget();

    float m_currentPosition;
    float m_xPosition;
    float m_yPosition;
    float m_spinSpeed;
    float m_targetSpeed;
    bool m_isSpinning;
    bool m_isAccelerating;
    int m_currentSymbol;
    int m_targetSymbol;
    float m_symbolHeight;
    float m_deceleration;
    std::vector<std::unique_ptr<Symbol>> m_symbols;
}; 