#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Symbol {
public:
    enum class Type {
        Circle,
        Square,
        Triangle,
        Star,
        Diamond
    };

    Symbol(int id, Type type, const sf::Color& color);

    void draw(sf::RenderWindow& window);
    
    void setPosition(const sf::Vector2f& pos);
    void setScale(const sf::Vector2f& scale);

    int getId() const { return m_id; }
    const sf::Vector2f& getPosition() const { return m_position; }

private:
    void createShape(Type type, const sf::Color& color);
    void createStar();
    void createDiamond();

    int m_id;
    sf::Vector2f m_position;
    sf::Vector2f m_scale;
    std::unique_ptr<sf::Shape> m_shape;
}; 