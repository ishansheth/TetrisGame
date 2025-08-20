#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class DisplayContainer;

class IShape
{
  public:
    virtual std::vector<sf::RectangleShape **> getShapeContainer() = 0;
    virtual void handleKey(const sf::Keyboard::Key &k) = 0;
    virtual void drawShape(sf::RenderWindow &displayWindow) = 0;
    virtual bool moveShape() = 0;
    virtual bool getMoveStatus() = 0;
    virtual bool isShapeBroken() = 0;
    virtual void setBroken() = 0;
    virtual void setPosition(sf::Vector2f location) = 0;
    virtual bool isBomb() = 0;
    virtual ~IShape() = default;
};