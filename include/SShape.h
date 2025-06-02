#pragma once
#include "DisplayContainer.h"
#include "IShape.h"
#include "Util.h"
#include <SFML/Graphics.hpp>


struct SShape : public IShape {
    // s shape
    //      _________
    //      | 1 | 2 |
    //  | 3 | 4 |

    public:
    sf::RectangleShape* rectangle1;
    sf::RectangleShape* rectangle2;
    sf::RectangleShape* rectangle3;
    sf::RectangleShape* rectangle4;
    std::vector<sf::RectangleShape*> shapeContainer;
    sf::Color sqaureColor  = sf::Color (100, 250, 50);
    sf::Color outlineColor = sf::Color (255, 255, 255);
    float outlineThickness = 3;
    sf::Vector2f shapeVelocity;
    sf::Vector2f shapeCenter;
    DisplayContainer* dContainer;
    bool isMoving;
    bool isBroken;

    public:
    SShape (DisplayContainer* displayManager)
    : dContainer (displayManager), rectangle1 (new sf::RectangleShape ()),
      rectangle2 (new sf::RectangleShape ()), rectangle3 (new sf::RectangleShape ()),
      rectangle4 (new sf::RectangleShape ()) {
        shapeVelocity = sf::Vector2f (SHAPE_DOWN_FALL_SPEED_X, SHAPE_DOWN_FALL_SPEED_Y);

        // in the beginning all shapes are moving
        isMoving = true;
        isBroken = false;

        rectangle1->setSize (sf::Vector2f (SQUARE_SIDE_LENGTH, SQUARE_SIDE_LENGTH));
        rectangle2->setSize (sf::Vector2f (SQUARE_SIDE_LENGTH, SQUARE_SIDE_LENGTH));
        rectangle3->setSize (sf::Vector2f (SQUARE_SIDE_LENGTH, SQUARE_SIDE_LENGTH));
        rectangle4->setSize (sf::Vector2f (SQUARE_SIDE_LENGTH, SQUARE_SIDE_LENGTH));

        rectangle1->setFillColor (sqaureColor);
        rectangle1->setOutlineColor (outlineColor);
        rectangle1->setOutlineThickness (outlineThickness);
        rectangle2->setFillColor (sqaureColor);
        rectangle2->setOutlineColor (outlineColor);
        rectangle2->setOutlineThickness (outlineThickness);
        rectangle3->setFillColor (sqaureColor);
        rectangle3->setOutlineColor (outlineColor);
        rectangle3->setOutlineThickness (outlineThickness);
        rectangle4->setFillColor (sqaureColor);
        rectangle4->setOutlineColor (outlineColor);
        rectangle4->setOutlineThickness (outlineThickness);

        shapeContainer.push_back (rectangle1);
        shapeContainer.push_back (rectangle2);
        shapeContainer.push_back (rectangle3);
        shapeContainer.push_back (rectangle4);
    }

    virtual ~SShape () {
        delete rectangle1;
        delete rectangle2;
        delete rectangle3;
        delete rectangle4;
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
        location1.x = location1.x + (SQUARE_SIDE_LENGTH) + 3 * SQUARE_OUTLINE_THICKNESS;
        location1.y = location1.y + SQUARE_OUTLINE_THICKNESS;
        rectangle1->setPosition (location1);

        location1 = location;
        location1.x = location1.x + 2 * (SQUARE_SIDE_LENGTH) + 5 * SQUARE_OUTLINE_THICKNESS;
        location1.y = location1.y + SQUARE_OUTLINE_THICKNESS;
        rectangle2->setPosition (location1);


        location1   = location;
        location1.x = location1.x + SQUARE_OUTLINE_THICKNESS;
        location1.y = location1.y + (SQUARE_SIDE_LENGTH) + 3 * SQUARE_OUTLINE_THICKNESS;
        rectangle3->setPosition (location1);

        location1 = location;
        location1.y = location1.y + (SQUARE_SIDE_LENGTH) + 3 * SQUARE_OUTLINE_THICKNESS;
        location1.x = location1.x + SQUARE_SIDE_LENGTH + 3 * SQUARE_OUTLINE_THICKNESS;

        // shapecenter is at 4th rectangle upper right corner
        shapeCenter.x = location1.x;
        shapeCenter.y = location1.y;

        rectangle4->setPosition (location1);
    }

    virtual void drawAsNextShape (sf::RenderWindow& displayWindow) override {
        displayWindow.draw (*rectangle1);
        displayWindow.draw (*rectangle2);
        displayWindow.draw (*rectangle3);
        displayWindow.draw (*rectangle4);
    }

    virtual void drawShape (sf::RenderWindow& displayWindow) override {
        // set the velocity vector
        shapeVelocity.x = 0;
        shapeVelocity.y = 0.1f;

        if (rectangle1 != nullptr) {
            auto p1 = rectangle1->getPosition () + shapeVelocity;
            if (isWithinDrawWindow (p1) && !dContainer->isIntersecting (p1)) {
                shapeCenter += shapeVelocity;
                rectangle1->move (shapeVelocity);
                displayWindow.draw (*rectangle1);
            } else {
                isMoving = false;
            }
        }

        if (rectangle2 != nullptr) {
            auto p2 = rectangle2->getPosition () + shapeVelocity;
            if (isWithinDrawWindow (p2) && !dContainer->isIntersecting (p2)) {
                rectangle2->move (shapeVelocity);
                displayWindow.draw (*rectangle2);
            } else {
                isMoving = false;
            }
        }

        if (rectangle3 != nullptr) {
            auto p3 = rectangle3->getPosition () + shapeVelocity;
            if (isWithinDrawWindow (p3) && !dContainer->isIntersecting (p3)) {
                rectangle3->move (shapeVelocity);
                displayWindow.draw (*rectangle3);
            } else {
                isMoving = false;
            }
        }

        if (rectangle4 != nullptr) {
            auto p4 = rectangle4->getPosition () + shapeVelocity;
            if (isWithinDrawWindow (p4) && !dContainer->isIntersecting (p4)) {
                rectangle4->move (shapeVelocity);
                displayWindow.draw (*rectangle4);
            } else {
                isMoving = false;
            }
        }
    }

    virtual void handleKey (sf::Keyboard::Key k) override {
        if (!isMoving)
            return;

        // square shape does not need rotation
        if (sf::Keyboard::Left == k) {
            // rotate shape velocity vector to <- direction and move within window
            shapeVelocity.x = -26.f;
            shapeVelocity.y = 0;

            auto p1 = rectangle1->getPosition () + shapeVelocity;
            auto p2 = rectangle2->getPosition () + shapeVelocity;
            auto p3 = rectangle3->getPosition () + shapeVelocity;
            auto p4 = rectangle4->getPosition () + shapeVelocity;

            if (isWithinDrawWindow (p1) && isWithinDrawWindow (p2) &&
            isWithinDrawWindow (p3) && isWithinDrawWindow (p4) &&
            !dContainer->isIntersecting (p1) && !dContainer->isIntersecting (p2) &&
            !dContainer->isIntersecting (p3) && !dContainer->isIntersecting (p4)) {
                rectangle1->move (shapeVelocity);
                rectangle2->move (shapeVelocity);
                rectangle3->move (shapeVelocity);
                rectangle4->move (shapeVelocity);
                // shift the center
                shapeCenter += shapeVelocity;
            }
        } else if (sf::Keyboard::Right == k) {
            // rotate shape velocity vector to -> direction and move within window
            shapeVelocity.x = 26.f;
            shapeVelocity.y = 0;

            auto p1 = rectangle1->getPosition () + shapeVelocity;
            auto p2 = rectangle2->getPosition () + shapeVelocity;
            auto p3 = rectangle3->getPosition () + shapeVelocity;
            auto p4 = rectangle4->getPosition () + shapeVelocity;

            if (isWithinDrawWindow (p1) && isWithinDrawWindow (p2) &&
            isWithinDrawWindow (p3) && isWithinDrawWindow (p4) &&
            !dContainer->isIntersecting (p1) && !dContainer->isIntersecting (p2) &&
            !dContainer->isIntersecting (p3) && !dContainer->isIntersecting (p4)) {
                rectangle1->move (shapeVelocity);
                rectangle2->move (shapeVelocity);
                rectangle3->move (shapeVelocity);
                rectangle4->move (shapeVelocity);
                // shift the center
                shapeCenter += shapeVelocity;
            }

        } else if (sf::Keyboard::Space == k) {
            // rotate clockwise 90 degrees rectangle3
            auto vec3 = rectangle3->getPosition () - shapeCenter;
            auto temp = vec3.x;
            vec3.x    = static_cast<int> (-vec3.y);
            vec3.y    = static_cast<int> (temp);
            vec3 += shapeCenter;

            // rotate clockwise 90 degrees rectangle2
            auto vec2 = rectangle2->getPosition () - shapeCenter;
            temp      = vec2.x;
            vec2.x    = static_cast<int> (-vec2.y);
            vec2.y    = static_cast<int> (temp);
            vec2 += shapeCenter;

            // rotate clockwise 90 degrees rectangle2
            auto vec4 = rectangle4->getPosition () - shapeCenter;
            temp      = vec4.x;
            vec4.x    = static_cast<int> (-vec4.y);
            vec4.y    = static_cast<int> (temp);
            vec4 += shapeCenter;

            // rotate clockwise 90 degrees rectangle2
            auto vec1 = rectangle1->getPosition () - shapeCenter;
            temp      = vec1.x;
            vec1.x    = static_cast<int> (-vec1.y);
            vec1.y    = static_cast<int> (temp);
            vec1 += shapeCenter;
            if (isWithinDrawWindow (vec1) && isWithinDrawWindow (vec2) &&
            isWithinDrawWindow (vec3) && isWithinDrawWindow (vec4)) {
                rectangle1->setPosition (vec1);
                rectangle2->setPosition (vec2);
                rectangle3->setPosition (vec3);
                rectangle4->setPosition (vec4);
            }
        }
    }
};
