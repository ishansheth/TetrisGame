#pragma once
#include "DisplayContainer.h"
#include "GameConstants.h"
#include "IShape.h"
#include "Util.h"
#include <SFML/Graphics.hpp>

class BaseShape : public IShape
{
  protected:
    sf::Texture shapeTexture;
    DisplayContainer *dContainer;
    std::vector<sf::RectangleShape **> shapeContainer;

    const sf::Color outlineColor = sf::Color(255, 255, 255);
    sf::Vector2f shapeVelocity;

    bool isMoving;
    bool isBroken;
    sf::Color sqaureColor;

    sf::RectangleShape *rectangle1;
    sf::RectangleShape *rectangle2;
    sf::RectangleShape *rectangle3;
    sf::RectangleShape *rectangle4;

    sf::Vector2f shapeCenter;

  public:
    BaseShape(DisplayContainer *displayManager, sf::Color color)
        : dContainer(displayManager), sqaureColor(color), rectangle1(new sf::RectangleShape()),
          rectangle2(new sf::RectangleShape()), rectangle3(new sf::RectangleShape()),
          rectangle4(new sf::RectangleShape())
    {
        shapeVelocity = sf::Vector2f(SHAPE_DOWN_FALL_SPEED_X, SHAPE_DOWN_FALL_SPEED_Y);
        shapeTexture.loadFromFile(std::getenv("HOME") + std::string(TOSTRINGYFY(TEXTURE_FILE_PATH)),
                                  sf::IntRect({0, 0}, {32, 32}));
        isMoving = true;
        isBroken = false;

        rectangle1->setSize(sf::Vector2f(SQUARE_SIDE_LENGTH, SQUARE_SIDE_LENGTH));
        rectangle2->setSize(sf::Vector2f(SQUARE_SIDE_LENGTH, SQUARE_SIDE_LENGTH));
        rectangle3->setSize(sf::Vector2f(SQUARE_SIDE_LENGTH, SQUARE_SIDE_LENGTH));
        rectangle4->setSize(sf::Vector2f(SQUARE_SIDE_LENGTH, SQUARE_SIDE_LENGTH));

        rectangle1->setFillColor(sqaureColor);
        rectangle1->setOutlineColor(outlineColor);
        rectangle1->setOutlineThickness(SQUARE_OUTLINE_THICKNESS);
        rectangle1->setTexture(&shapeTexture);

        rectangle2->setFillColor(sqaureColor);
        rectangle2->setOutlineColor(outlineColor);
        rectangle2->setOutlineThickness(SQUARE_OUTLINE_THICKNESS);
        rectangle2->setTexture(&shapeTexture);

        rectangle3->setFillColor(sqaureColor);
        rectangle3->setOutlineColor(outlineColor);
        rectangle3->setOutlineThickness(SQUARE_OUTLINE_THICKNESS);
        rectangle3->setTexture(&shapeTexture);

        rectangle4->setFillColor(sqaureColor);
        rectangle4->setOutlineColor(outlineColor);
        rectangle4->setOutlineThickness(SQUARE_OUTLINE_THICKNESS);
        rectangle4->setTexture(&shapeTexture);

        shapeContainer.push_back(&rectangle1);
        shapeContainer.push_back(&rectangle2);
        shapeContainer.push_back(&rectangle3);
        shapeContainer.push_back(&rectangle4);

        shapeVelocity.x = SHAPE_DOWN_FALL_SPEED_X;
        shapeVelocity.y = SHAPE_DOWN_FALL_SPEED_Y;
    }

    virtual ~BaseShape()
    {
        delete rectangle1;
        delete rectangle2;
        delete rectangle3;
        delete rectangle4;
        shapeContainer.clear();
    }

    virtual bool canShapeFall() const override
    {
        return true;
    }

    std::vector<sf::RectangleShape **> getShapeContainer() const override
    {
        return shapeContainer;
    }

    virtual bool getMoveStatus() const override
    {
        return isMoving;
    }

    virtual bool isShapeBroken() const override
    {
        return isBroken;
    }

    virtual void setBroken() override
    {
        isBroken = true;
        if (rectangle1 != nullptr)
        {
            rectangle1->setFillColor(sf::Color(129, 133, 137));
            rectangle1->setTexture(nullptr);
        }

        if (rectangle2 != nullptr)
        {
            rectangle2->setFillColor(sf::Color(129, 133, 137));
            rectangle2->setTexture(nullptr);
        }

        if (rectangle3 != nullptr)
        {
            rectangle3->setFillColor(sf::Color(129, 133, 137));
            rectangle3->setTexture(nullptr);
        }

        if (rectangle4 != nullptr)
        {
            rectangle4->setFillColor(sf::Color(129, 133, 137));
            rectangle4->setTexture(nullptr);
        }

    }

    virtual void drawShape(sf::RenderWindow &displayWindow) override
    {
        // set the velocity vector
        if (rectangle1 != nullptr)
            displayWindow.draw(*rectangle1);

        if (rectangle2 != nullptr)
            displayWindow.draw(*rectangle2);

        if (rectangle3 != nullptr)
            displayWindow.draw(*rectangle3);

        if (rectangle4 != nullptr)
            displayWindow.draw(*rectangle4);
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

            const auto p1 = rectangle1->getPosition() + shapeVelocity;
            const auto p2 = rectangle2->getPosition() + shapeVelocity;
            const auto p3 = rectangle3->getPosition() + shapeVelocity;
            const auto p4 = rectangle4->getPosition() + shapeVelocity;

            if (isWithinDrawWindow(p1) && isWithinDrawWindow(p2) && isWithinDrawWindow(p3) && isWithinDrawWindow(p4) &&
                !dContainer->isIntersecting(p1, this) && !dContainer->isIntersecting(p2, this) &&
                !dContainer->isIntersecting(p3, this) && !dContainer->isIntersecting(p4, this))
            {
                rectangle1->move(shapeVelocity);
                rectangle2->move(shapeVelocity);
                rectangle3->move(shapeVelocity);
                rectangle4->move(shapeVelocity);
                // shift the center
                shapeCenter += shapeVelocity;
            }
        }
        else if (sf::Keyboard::Right == k)
        {
            // rotate shape velocity vector to -> direction and move within window
            shapeVelocity.x = SQUARE_SIDE_LENGTH_WITH_OUTLINE;
            shapeVelocity.y = 0;

            const auto p1 = rectangle1->getPosition() + shapeVelocity;
            const auto p2 = rectangle2->getPosition() + shapeVelocity;
            const auto p3 = rectangle3->getPosition() + shapeVelocity;
            const auto p4 = rectangle4->getPosition() + shapeVelocity;

            if (isWithinDrawWindow(p1) && isWithinDrawWindow(p2) && isWithinDrawWindow(p3) && isWithinDrawWindow(p4) &&
                !dContainer->isIntersecting(p1, this) && !dContainer->isIntersecting(p2, this) &&
                !dContainer->isIntersecting(p3, this) && !dContainer->isIntersecting(p4, this))
            {
                rectangle1->move(shapeVelocity);
                rectangle2->move(shapeVelocity);
                rectangle3->move(shapeVelocity);
                rectangle4->move(shapeVelocity);
                // shift the center
                shapeCenter += shapeVelocity;
            }
        }
        else if (sf::Keyboard::Space == k)
        {
            // rotate clockwise 90 degrees rectangle3
            auto vec3 = rectangle3->getPosition() - shapeCenter;
            auto temp = vec3.x;
            vec3.x = static_cast<int>(-vec3.y);
            vec3.y = static_cast<int>(temp);
            vec3 += shapeCenter;

            // rotate clockwise 90 degrees rectangle2
            auto vec2 = rectangle2->getPosition() - shapeCenter;
            temp = vec2.x;
            vec2.x = static_cast<int>(-vec2.y);
            vec2.y = static_cast<int>(temp);
            vec2 += shapeCenter;

            // rotate clockwise 90 degrees rectangle2
            auto vec4 = rectangle4->getPosition() - shapeCenter;
            temp = vec4.x;
            vec4.x = static_cast<int>(-vec4.y);
            vec4.y = static_cast<int>(temp);
            vec4 += shapeCenter;

            // rotate clockwise 90 degrees rectangle2
            auto vec1 = rectangle1->getPosition() - shapeCenter;
            temp = vec1.x;
            vec1.x = static_cast<int>(-vec1.y);
            vec1.y = static_cast<int>(temp);
            vec1 += shapeCenter;
            if (isWithinDrawWindow(vec1) && 
                isWithinDrawWindow(vec2) && 
                isWithinDrawWindow(vec3) &&
                isWithinDrawWindow(vec4) && 
                !dContainer->isIntersecting(vec1, this) && 
                !dContainer->isIntersecting(vec2, this) &&
                !dContainer->isIntersecting(vec3, this) && 
                !dContainer->isIntersecting(vec4, this)
            )
            {
                rectangle1->setPosition(vec1);
                rectangle2->setPosition(vec2);
                rectangle3->setPosition(vec3);
                rectangle4->setPosition(vec4);
            }
        }
    }

    virtual bool moveShape() override
    {
        // either move the whole shape or move individual cubes

        // shape falling as a whole
        shapeVelocity.x = SHAPE_DOWN_FALL_SPEED_X;
        shapeVelocity.y = SHAPE_DOWN_FALL_SPEED_Y;
        bool movestatus = false;

        if (rectangle1 != nullptr && rectangle2 != nullptr && rectangle3 != nullptr && rectangle4 != nullptr)
        {
            auto p1 = rectangle1->getPosition() + shapeVelocity;
            auto p2 = rectangle2->getPosition() + shapeVelocity;
            auto p3 = rectangle3->getPosition() + shapeVelocity;
            auto p4 = rectangle4->getPosition() + shapeVelocity;

            if (isWithinDrawWindow(p1) && isWithinDrawWindow(p2) && isWithinDrawWindow(p3) && isWithinDrawWindow(p4) &&
                !dContainer->isIntersecting(p1, this) && !dContainer->isIntersecting(p2, this) &&
                !dContainer->isIntersecting(p3, this) && !dContainer->isIntersecting(p4, this))
            {
                rectangle1->move(shapeVelocity);
                rectangle2->move(shapeVelocity);
                rectangle3->move(shapeVelocity);
                rectangle4->move(shapeVelocity);
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