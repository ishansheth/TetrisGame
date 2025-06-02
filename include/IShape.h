#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class IShape
{
    public:
    virtual std::vector<sf::RectangleShape*> getShapeContianer() = 0;
    virtual void handleKey(sf::Keyboard::Key k) = 0;
    virtual void drawShape(sf::RenderWindow& displayWindow) = 0;
    virtual bool getMoveStatus() = 0;
    virtual void drawAsNextShape(sf::RenderWindow& displayWindow) = 0;
    virtual bool isShapeBroken() = 0;
    virtual void setBroken() = 0;
    virtual ~IShape() = default;

};