#pragma once
#include "IShape.h"
#include "GameConstants.h"
#include <SFML/Graphics.hpp>

class SingleSquare : public IShape
{
    sf::RectangleShape *rectangle;
    sf::Texture shapeTexture;
    sf::Color sqaureColor;
    const sf::Color outlineColor = sf::Color(255, 255, 255);
    std::vector<sf::RectangleShape **> shapeContainer;

    public:
    SingleSquare(sf::Color color): rectangle(new sf::RectangleShape()), sqaureColor(color)
    {
        shapeTexture.loadFromFile(std::getenv("HOME") + std::string(TOSTRINGYFY(TEXTURE_FILE_PATH)),
                                  sf::IntRect({0, 0}, {32, 32}));

        rectangle->setSize(sf::Vector2f(SQUARE_SIDE_LENGTH, SQUARE_SIDE_LENGTH));
        rectangle->setFillColor(sqaureColor);
        rectangle->setOutlineColor(outlineColor);
        rectangle->setOutlineThickness(SQUARE_OUTLINE_THICKNESS);
        rectangle->setTexture(&shapeTexture);
        shapeContainer.push_back(&rectangle);
    }

    ~SingleSquare()
    {
        delete rectangle;
        shapeContainer.clear();
    }

    virtual bool isBomb() override
    {
        return false;
    }    

    virtual void setPosition(sf::Vector2f location) override
    {
        rectangle->setPosition(location);
    }

    std::vector<sf::RectangleShape **> getShapeContainer() override
    {
        return shapeContainer;
    }

    virtual bool getMoveStatus() override
    {
        return false;
    }

    virtual bool isShapeBroken() override
    {
        return true;
    }

    virtual void setBroken() override
    {
    }

    virtual void drawShape(sf::RenderWindow &displayWindow) override
    {
        if (rectangle != nullptr)
            displayWindow.draw(*rectangle);

    }

    virtual void handleKey(const sf::Keyboard::Key &k) override
    {}

    virtual bool moveShape() override
    {
        return false;
    }


};