#pragma once
#include "BaseShape.h"
#include "DisplayContainer.h"
#include "Util.h"
#include <SFML/Graphics.hpp>

struct SShape : public BaseShape
{
    // s shape
    //      _________
    //      | 1 | 2 |
    //  | 3 | 4 |

  public:
    SShape(DisplayContainer *displayManager) : BaseShape(displayManager, sf::Color(25, 10, 250))
    {
    }

    IShape *clone(DisplayContainer *displayManager) override
    {
        SShape *clonedObj = new SShape(displayManager);
        return clonedObj;
    }

    virtual ~SShape()
    {
    }

    virtual bool isBomb() override
    {
        return false;
    }

    virtual void setposition(sf::Vector2f location) override
    {
        auto location1 = location;
        location1.x = location1.x + (SQUARE_SIDE_LENGTH) + 3 * SQUARE_OUTLINE_THICKNESS;
        location1.y = location1.y + SQUARE_OUTLINE_THICKNESS;
        rectangle1->setPosition(location1);

        location1 = location;
        location1.x = location1.x + 2 * (SQUARE_SIDE_LENGTH) + 5 * SQUARE_OUTLINE_THICKNESS;
        location1.y = location1.y + SQUARE_OUTLINE_THICKNESS;
        rectangle2->setPosition(location1);

        location1 = location;
        location1.x = location1.x + SQUARE_OUTLINE_THICKNESS;
        location1.y = location1.y + (SQUARE_SIDE_LENGTH) + 3 * SQUARE_OUTLINE_THICKNESS;
        rectangle3->setPosition(location1);

        location1 = location;
        location1.y = location1.y + (SQUARE_SIDE_LENGTH) + 3 * SQUARE_OUTLINE_THICKNESS;
        location1.x = location1.x + SQUARE_SIDE_LENGTH + 3 * SQUARE_OUTLINE_THICKNESS;

        // shapecenter is at 4th rectangle upper right corner
        shapeCenter.x = location1.x;
        shapeCenter.y = location1.y;

        rectangle4->setPosition(location1);
    }
};
