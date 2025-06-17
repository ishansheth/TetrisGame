#pragma once
#include "DisplayContainer.h"
#include "IShape.h"
#include "Util.h"
#include <SFML/Graphics.hpp>

class BombShape : public IShape {
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
    std::vector<sf::RectangleShape*> shapeContainer;
    sf::Color sqaureColor = sf::Color (100, 250, 50);

    sf::Color outlineColor = sf::Color (255, 255, 255);
    sf::Vector2f shapeVelocity;
    bool isMoving;
    bool isBroken;

    public:

    BombShape (DisplayContainer* displayManager)
    : dContainer (displayManager), rectangle1 (new sf::RectangleShape ()),
      rectangle2 (new sf::RectangleShape ()), rectangle3 (new sf::RectangleShape ()),
      rectangle4 (new sf::RectangleShape ()), rectangle5 (new sf::RectangleShape ()) {
        shapeVelocity = sf::Vector2f (SHAPE_DOWN_FALL_SPEED_X, SHAPE_DOWN_FALL_SPEED_Y);
        isMoving = true;
        isBroken = false;

        rectangle1->setSize (sf::Vector2f (SQUARE_SIDE_LENGTH, SQUARE_SIDE_LENGTH));
        rectangle2->setSize (sf::Vector2f (SQUARE_SIDE_LENGTH, SQUARE_SIDE_LENGTH));
        rectangle3->setSize (sf::Vector2f (SQUARE_SIDE_LENGTH, SQUARE_SIDE_LENGTH));
        rectangle4->setSize (sf::Vector2f (SQUARE_SIDE_LENGTH, SQUARE_SIDE_LENGTH));
        rectangle5->setSize (sf::Vector2f (SQUARE_SIDE_LENGTH, SQUARE_SIDE_LENGTH));

        rectangle1->setFillColor (sqaureColor);
        rectangle1->setOutlineColor (outlineColor);
        rectangle1->setOutlineThickness (SQUARE_OUTLINE_THICKNESS);

        rectangle2->setFillColor (sqaureColor);
        rectangle2->setOutlineColor (outlineColor);
        rectangle2->setOutlineThickness (SQUARE_OUTLINE_THICKNESS);

        rectangle3->setFillColor (sqaureColor);
        rectangle3->setOutlineColor (outlineColor);
        rectangle3->setOutlineThickness (SQUARE_OUTLINE_THICKNESS);

        rectangle4->setFillColor (sqaureColor);
        rectangle4->setOutlineColor (outlineColor);
        rectangle4->setOutlineThickness (SQUARE_OUTLINE_THICKNESS);

        rectangle5->setFillColor (sqaureColor);
        rectangle5->setOutlineColor (outlineColor);
        rectangle5->setOutlineThickness (SQUARE_OUTLINE_THICKNESS);

      }

    virtual IShape* clone(DisplayContainer* displayManager)
    {
        BombShape* clonedObj = new BombShape(displayManager);
        return clonedObj;

    }

    virtual ~BombShape()
    {
        delete rectangle1;
        delete rectangle2;
        delete rectangle3;
        delete rectangle4;
        delete rectangle5;
        shapeContainer.clear ();

    }

    std::vector<sf::RectangleShape*> getShapeContianer () {
        return shapeContainer;
    }

    virtual bool getMoveStatus () override {
        return isMoving;
    }

    virtual bool isShapeBroken () override {
        return isBroken;
    }

    virtual void setBroken () override {
        isBroken = true;
    }

    void setposition (sf::Vector2f location) {
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

    virtual void moveShape() override {
        // either move the whole shape or move individual cubes

        if(rectangle1 != nullptr && 
        rectangle2 != nullptr && 
        rectangle3 != nullptr && 
        rectangle4 != nullptr)
        {
            auto p1 = rectangle1->getPosition () + shapeVelocity;
            auto p2 = rectangle2->getPosition () + shapeVelocity;
            auto p3 = rectangle3->getPosition () + shapeVelocity;
            auto p4 = rectangle4->getPosition () + shapeVelocity;

            if (isWithinDrawWindow (p1) && 
            isWithinDrawWindow (p2) &&
            isWithinDrawWindow (p3) && 
            isWithinDrawWindow (p4) &&
            !dContainer->isIntersecting (p1) && 
            !dContainer->isIntersecting (p2) &&
            !dContainer->isIntersecting (p3) && 
            !dContainer->isIntersecting (p4)) 
            {
                rectangle1->move (shapeVelocity);
                rectangle2->move (shapeVelocity);
                rectangle3->move (shapeVelocity);
                rectangle4->move (shapeVelocity);
            }
        }

        dropShape();
    }

    void dropShape()
    {
        // set the velocity vector
        shapeVelocity.x = SHAPE_DOWN_FALL_SPEED_X;
        shapeVelocity.y = SHAPE_DOWN_FALL_SPEED_Y;

        if (rectangle1 != nullptr) {
            auto p1 = rectangle1->getPosition () + shapeVelocity;
            if (isWithinDrawWindow (p1) && !dContainer->isIntersecting (p1)) {
                rectangle1->move (shapeVelocity);
            } else {
                isMoving = false;
            }
        }

        if (rectangle2 != nullptr) {
            auto p2 = rectangle2->getPosition () + shapeVelocity;
            if (isWithinDrawWindow (p2) && !dContainer->isIntersecting (p2)) {
                rectangle2->move (shapeVelocity);
            } else {
                isMoving = false;
            }
        }

        if (rectangle3 != nullptr) {
            auto p3 = rectangle3->getPosition () + shapeVelocity;
            if (isWithinDrawWindow (p3) && !dContainer->isIntersecting (p3)) {
                rectangle3->move (shapeVelocity);
            } else {
                isMoving = false;
            }
        }

        if (rectangle4 != nullptr) {
            auto p4 = rectangle4->getPosition () + shapeVelocity;
            if (isWithinDrawWindow (p4) && !dContainer->isIntersecting (p4)) {
                rectangle4->move (shapeVelocity);
            } else {
                isMoving = false;
            }
        }
    }


};