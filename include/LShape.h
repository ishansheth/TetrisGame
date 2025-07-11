#pragma once
#include "DisplayContainer.h"
#include "BaseShape.h"
#include "Util.h"
#include <SFML/Graphics.hpp>

struct LShape : public BaseShape {
    // L shape
    // | 1 |
    // | 2 |
    // | 3 | 4 |

    public:
    LShape (DisplayContainer* displayManager) : 
    BaseShape(displayManager, sf::Color (100, 250, 50))
    {}

    IShape* clone(DisplayContainer* displayManager) override
    {
        LShape* clonedObj = new LShape(displayManager);
        return clonedObj;
    }


    virtual ~LShape () 
    {}

    virtual bool isBomb() override
    {
        return false;
    }

    virtual void setposition (sf::Vector2f location) override
    {
        auto location1 = location;
        location1.x    = location1.x + SQUARE_OUTLINE_THICKNESS;
        location1.y    = location1.y + SQUARE_OUTLINE_THICKNESS;
        rectangle1->setPosition (location1);

        location1 = location;
        location1.y = location1.y + (SQUARE_SIDE_LENGTH) + 3 * SQUARE_OUTLINE_THICKNESS;
        location1.x = location1.x + SQUARE_OUTLINE_THICKNESS;
        rectangle2->setPosition (location1);

        location1 = location;
        location1.y = location1.y + (2 * SQUARE_SIDE_LENGTH) + 5 * SQUARE_OUTLINE_THICKNESS;
        location1.x = location1.x + SQUARE_OUTLINE_THICKNESS;
        rectangle3->setPosition (location1);

        // shapecenter is at 3rd rectangle upper right corner
        shapeCenter.y = location1.y;
        shapeCenter.x = location1.x;

        location1 = location;
        location1.x = location1.x + (SQUARE_SIDE_LENGTH) + 3 * SQUARE_OUTLINE_THICKNESS;
        location1.y = location1.y + (2 * SQUARE_SIDE_LENGTH) + 5 * SQUARE_OUTLINE_THICKNESS;
        rectangle4->setPosition (location1);
    }



};
