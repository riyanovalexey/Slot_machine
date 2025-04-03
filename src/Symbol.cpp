#include "Symbol.h"
#include <cmath>
#include "GameConstants.h"

Symbol::Symbol(int id, Type type, const sf::Color& color)
    : m_id(id)
    , m_position(0.f, 0.f)
    , m_scale(1.f, 1.f)
{
    createShape(type, color);
}

void Symbol::draw(sf::RenderWindow& window) {
    window.draw(*m_shape);
}

void Symbol::setPosition(const sf::Vector2f& pos) {
    m_position = pos;
    m_shape->setPosition(pos);
}

void Symbol::setScale(const sf::Vector2f& scale) {
    m_scale = scale;
    m_shape->setScale(scale);
}

void Symbol::createShape(Type type, const sf::Color& color) {
    const float size = SymbolConstants::SYMBOL_SIZE;

    switch (type) {
        case Type::Circle: {
            auto circle = std::make_unique<sf::CircleShape>(size / 2);
            circle->setFillColor(color);
            circle->setOrigin(sf::Vector2f(size / 2, size / 2));
            m_shape = std::move(circle);
            break;
        }
        case Type::Square: {
            auto square = std::make_unique<sf::RectangleShape>(sf::Vector2f(size, size));
            square->setFillColor(color);
            square->setOrigin(sf::Vector2f(size / 2, size / 2));
            m_shape = std::move(square);
            break;
        }
        case Type::Triangle: {
            auto triangle = std::make_unique<sf::CircleShape>(size / 2, 3);
            triangle->setFillColor(color);
            triangle->setOrigin(sf::Vector2f(size / 2, size / 2));
            m_shape = std::move(triangle);
            break;
        }
        case Type::Star:
            createStar();
            m_shape->setFillColor(color);
            break;
        case Type::Diamond:
            createDiamond();
            m_shape->setFillColor(color);
            break;
    }
}

void Symbol::createStar() {
    const float size = SymbolConstants::SYMBOL_SIZE;
    const int points = SymbolConstants::STAR_POINTS;
    const float innerRadius = size * SymbolConstants::STAR_INNER_RADIUS_RATIO;
    const float outerRadius = size * SymbolConstants::STAR_OUTER_RADIUS_RATIO;

    auto star = std::make_unique<sf::ConvexShape>();
    star->setPointCount(points * 2);

    for (int i = 0; i < points * 2; ++i) {
        float radius = (i % 2 == 0) ? outerRadius : innerRadius;
        float angle = i * SymbolConstants::PI / points;
        float x = std::cos(angle) * radius;
        float y = std::sin(angle) * radius;
        star->setPoint(i, sf::Vector2f(x + outerRadius, y + outerRadius));
    }

    star->setOrigin(sf::Vector2f(size / 2, size / 2));
    m_shape = std::move(star);
}

void Symbol::createDiamond() {
    const float size = SymbolConstants::SYMBOL_SIZE;
    
    auto diamond = std::make_unique<sf::ConvexShape>();
    diamond->setPointCount(4);
    
    diamond->setPoint(0, sf::Vector2f(size / 2, 0));
    diamond->setPoint(1, sf::Vector2f(size, size / 2));
    diamond->setPoint(2, sf::Vector2f(size / 2, size));
    diamond->setPoint(3, sf::Vector2f(0, size / 2));
    
    diamond->setOrigin(sf::Vector2f(size / 2, size / 2));
    m_shape = std::move(diamond);
} 