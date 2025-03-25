#pragma once

#include <SFML/Graphics.hpp>

namespace GameConstants {
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 600;
    
    constexpr int REEL_COUNT = 3;
    constexpr int SYMBOLS_PER_REEL = 5;
    constexpr int VISIBLE_SYMBOLS = 3;
    
    constexpr int DEFAULT_BALANCE = 1000;
    constexpr int DEFAULT_BET = 10;
    constexpr int MIN_BET = 5;
    constexpr int BET_INCREMENT = 5;
    
    constexpr int THREE_OF_A_KIND_MULTIPLIER = 10;
    constexpr int TWO_OF_A_KIND_MULTIPLIER = 2;
    
    constexpr float AUTO_STOP_TIME = 5.0f;
    constexpr float WIN_DISPLAY_TIME = 3.0f;
    constexpr float NO_WIN_DISPLAY_TIME = 1.0f;
}

namespace ReelConstants {
    constexpr float DEFAULT_REEL_SPACING = 120.0f;
    constexpr float REEL_SPACING_SCALE_BASE = 800.0f;
    constexpr float REELS_HEIGHT = 150.0f;
    
    constexpr float STOP_DELAY = 0.5f;
    
    constexpr float MAX_SPIN_SPEED = 1000.0f;
    constexpr float MIN_SPIN_SPEED = 200.0f;
    constexpr float ACCELERATION_RATE = 1500.0f;
    constexpr float BASE_SPIN_SPEED = 1000.0f;
    constexpr float SPEED_VARIATION = 100.0f;
}

namespace UIConstants {
    const sf::Vector2f SPIN_BUTTON_SIZE(150.f, 70.f);
    const sf::Vector2f STOP_BUTTON_SIZE(150.f, 70.f);
    const sf::Vector2f BET_BUTTON_SIZE(50.f, 50.f);
    
    const float RIGHT_MARGIN = 30.f;
    const float BOTTOM_MARGIN = 30.f;
    const float LEFT_MARGIN = 10.f;
    const float BET_BUTTONS_RIGHT_MARGIN = 60.f;
    const float BET_BUTTONS_BOTTOM_MARGIN = 20.f;
    const float BUTTON_VERTICAL_SPACING = 90.f;
    
    const sf::Color SPIN_BUTTON_COLOR(50, 150, 30);
    const sf::Color STOP_BUTTON_COLOR(255, 100, 100);
    const sf::Color BET_INCREASE_COLOR(100, 255, 100);
    const sf::Color BET_DECREASE_COLOR(255, 100, 100);
    const sf::Color ACTIVE_BUTTON_COLOR(100, 100, 200);
    const sf::Color INACTIVE_BUTTON_COLOR(100, 100, 100);
    
    const unsigned int BALANCE_TEXT_SIZE = 24;
    const unsigned int BET_TEXT_SIZE = 24;
    const unsigned int WIN_TEXT_SIZE = 32;
    const unsigned int BUTTON_TEXT_SIZE = 20;
    const unsigned int HINTS_TEXT_SIZE = 18;
    
    const sf::Vector2f BALANCE_TEXT_POSITION(10.f, 10.f);
    const sf::Vector2f BET_TEXT_POSITION(10.f, 40.f);
    
    const sf::Color BALANCE_TEXT_COLOR = sf::Color::White;
    const sf::Color BET_TEXT_COLOR = sf::Color::White;
    const sf::Color WIN_TEXT_COLOR = sf::Color::Yellow;
    const sf::Color BUTTON_TEXT_COLOR = sf::Color::Black;
    const sf::Color HINTS_TEXT_COLOR(180, 180, 180);
    
    const sf::Color WIN_COLOR(255, 215, 0);
    const sf::Color NO_WIN_COLOR(200, 200, 200);
    
    const sf::Color BACKGROUND_COLOR(50, 70, 100);
} 