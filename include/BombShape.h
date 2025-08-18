#pragma once
#include "BaseShape.h"
#include "DisplayContainer.h"
#include "Util.h"
#include <SFML/Graphics.hpp>

class BombShape : public BaseShape
{
    // Bomb shape

    sf::RectangleShape *rectangle5;
    sf::Texture shapeTexture;

    DisplayContainer *dContainer;
    std::vector<sf::RectangleShape **> shapeContainer;

    sf::Vector2f shapeVelocity;
    bool isMoving;

  public:
    BombShape(DisplayContainer *displayManager) : BaseShape(displayManager, sf::Color(100, 250, 50)),
        dContainer(displayManager)    ,
        rectangle5(new sf::RectangleShape()) 
    {
        shapeVelocity = sf::Vector2f(SHAPE_DOWN_FALL_SPEED_X, SHAPE_DOWN_FALL_SPEED_Y);
        shapeTexture.loadFromFile(std::getenv("HOME") + std::string(TOSTRINGYFY(BOMB_IMAGE)));

        isMoving = true;

        rectangle5->setSize(sf::Vector2f(BOMB_SQUARE_SIDE_LENGTH, BOMB_SQUARE_SIDE_LENGTH));
        rectangle5->setTexture(&shapeTexture);
        shapeContainer.push_back(&rectangle5);
    }

    virtual bool isBomb() override
    {
        return true;
    }

    std::vector<sf::RectangleShape **> getShapeContianer() override
    {
        return shapeContainer;
    }

    virtual ~BombShape()
    {
        delete rectangle5;
    }


    virtual void setposition(sf::Vector2f location) override
    {
        rectangle5->setPosition(location);
    }

    virtual void drawShape(sf::RenderWindow &displayWindow) override
    {
        // set the velocity vector
        if (rectangle5 != nullptr)
            displayWindow.draw(*rectangle5);
    }

    virtual void handleKey(const sf::Keyboard::Key &k) override
    {
        if (!isMoving)
            return;

        // square shape does not need rotation
        if (sf::Keyboard::Left == k)
        {
            // rotate shape velocity vector to <- direction and move within window
            shapeVelocity.x = SQUARE_SIDE_LENGTH_WITH_OUTLINE;
            shapeVelocity.x = -shapeVelocity.x;
            shapeVelocity.y = 0;

            const auto p5 = rectangle5->getPosition() + shapeVelocity;

            if (isWithinDrawWindow(p5) && !dContainer->isIntersecting(p5, this))
            {
                rectangle5->move(shapeVelocity);
                // shift the center
                shapeCenter += shapeVelocity;
            }
        }
        else if (sf::Keyboard::Right == k)
        {
            // rotate shape velocity vector to -> direction and move within window
            shapeVelocity.x = SQUARE_SIDE_LENGTH_WITH_OUTLINE;
            shapeVelocity.y = 0;

            const auto p5 = rectangle5->getPosition() + shapeVelocity;

            if (isWithinDrawWindow(p5) && !dContainer->isIntersecting(p5, this))
            {
                rectangle5->move(shapeVelocity);
                // shift the center
                shapeCenter += shapeVelocity;
            }
        }

    }

    virtual bool getMoveStatus() override
    {
        return isMoving;
    }


    virtual bool moveShape() override
    {
        // either move the whole shape or move individual cubes

        // shape falling as a whole
        shapeVelocity.x = SHAPE_DOWN_FALL_SPEED_X;
        shapeVelocity.y = SHAPE_DOWN_FALL_SPEED_Y;
        bool movestatus = false;

        if (rectangle5 != nullptr)
        {
            auto p5 = rectangle5->getPosition() + shapeVelocity;

            if (isWithinDrawWindow(p5) && !dContainer->isIntersecting(p5, this))
            {
                rectangle5->move(shapeVelocity);
                // shift the center
                shapeCenter += shapeVelocity;
                movestatus = true;
            }
            else
            {
                isMoving = false;
            }
        }

        return movestatus;
    }
    
};