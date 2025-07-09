#pragma once
#include "DisplayContainer.h"
#include "BaseShape.h"
#include "Util.h"
#include <SFML/Graphics.hpp>

class BombShape : public BaseShape {
    // Bomb shape

    //  this is a bomb of size 3x3 so it creates a 3x3 
    //  size of hole where ever it drops   
    //
    //  |1| |2|
    //   _|3|_
    //  |4| |5|
    //

    sf::RectangleShape* rectangle1;
    sf::RectangleShape* rectangle2;
    sf::RectangleShape* rectangle3;
    sf::RectangleShape* rectangle4;
    sf::RectangleShape* rectangle5;

    DisplayContainer* dContainer;
    std::vector<sf::RectangleShape**> shapeContainer;
    sf::Color sqaureColor = sf::Color (100, 250, 50);

    sf::Color outlineColor = sf::Color (255, 255, 255);
    sf::Vector2f shapeVelocity;
    bool isMoving;
    bool isBroken;

    public:

    BombShape (DisplayContainer* displayManager) : 
    BaseShape(displayManager, sf::Color(100, 250, 50))
    {}

    virtual IShape* clone(DisplayContainer* displayManager)
    {
        BombShape* clonedObj = new BombShape(displayManager);
        return clonedObj;
    }

    virtual bool isBomb() override
    {
        return true;
    }

    virtual ~BombShape()
    {}

    virtual void setposition (sf::Vector2f location) override
    {
        auto location1 = location;
        location1.x    = location1.x + SQUARE_OUTLINE_THICKNESS;
        location1.y    = location1.y + SQUARE_OUTLINE_THICKNESS;
        rectangle1->setPosition (location1);

        location1 = location;
        location1.x = location1.x + (2*SQUARE_SIDE_LENGTH) + (5*SQUARE_OUTLINE_THICKNESS);
        location1.y    = location1.y + SQUARE_OUTLINE_THICKNESS;
        rectangle2->setPosition (location1);

        location1 = location;
        location1.x = location1.x + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS;
        location1.y = location1.y + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS;
        rectangle3->setPosition (location1);

        location1 = location;
        location1.x = location1.x + SQUARE_OUTLINE_THICKNESS;
        location1.y = location1.y + (2*SQUARE_SIDE_LENGTH) + (5*SQUARE_OUTLINE_THICKNESS);
        rectangle4->setPosition (location1);

        location1 = location;
        location1.x = location1.x + (2*SQUARE_SIDE_LENGTH) + (5*SQUARE_OUTLINE_THICKNESS);
        location1.y = location1.y + (2*SQUARE_SIDE_LENGTH) + (5*SQUARE_OUTLINE_THICKNESS);
        rectangle5->setPosition (location1);

    }

    virtual void drawShape (sf::RenderWindow& displayWindow) override 
    {
        displayWindow.draw (*rectangle1);
        displayWindow.draw (*rectangle2);
        displayWindow.draw (*rectangle3);
        displayWindow.draw (*rectangle4);
        displayWindow.draw (*rectangle5);
    }


    virtual void handleKey (sf::Keyboard::Key k) override {
        if (!isMoving)
            return;

        // square shape does not need rotation
        if (sf::Keyboard::Left == k) {
            // rotate shape velocity vector to <- direction and move within window
            shapeVelocity.x = -SQUARE_SIDE_LENGTH_WITH_OUTLINE;
            shapeVelocity.x = -shapeVelocity.x;
            shapeVelocity.y = 0;

        } else if (sf::Keyboard::Right == k) {
            // rotate shape velocity vector to -> direction and move within window
            shapeVelocity.x = SQUARE_SIDE_LENGTH_WITH_OUTLINE;
            shapeVelocity.y = 0;
        }        
    }


};