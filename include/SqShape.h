#pragma once
#include "BaseShape.h"
#include "DisplayContainer.h"
#include "Util.h"
#include <SFML/Graphics.hpp>

struct SqShape : public BaseShape
{
    // square shape
    /*
    | 1 | 2 |
    | 3 | 4 |

    */

  public:
    SqShape(DisplayContainer *displayManager) : BaseShape(displayManager, sf::Color(100, 50, 250))
    {
    }

    virtual ~SqShape()
    {
    }

    virtual bool isBomb() override
    {
        return false;
    }

    virtual void setPosition(sf::Vector2f location) override
    {
        // shapecenter is at 1st rectangle upper left corner
        shapeCenter = location;
        auto location1 = location;
        location1.x = location1.x + SQUARE_OUTLINE_THICKNESS;
        location1.y = location1.y + SQUARE_OUTLINE_THICKNESS;
        rectangle1->setPosition(location1);

        location1 = location;
        location1.x = location1.x + SQUARE_SIDE_LENGTH + 3 * SQUARE_OUTLINE_THICKNESS;
        location1.y = location1.y + SQUARE_OUTLINE_THICKNESS;
        rectangle2->setPosition(location1);

        location1 = location;
        location1.x = location1.x + SQUARE_OUTLINE_THICKNESS;
        location1.y = location1.y + SQUARE_SIDE_LENGTH + 3 * SQUARE_OUTLINE_THICKNESS;
        rectangle3->setPosition(location1);

        location1 = location;
        location1.x = location1.x + SQUARE_SIDE_LENGTH + 3 * SQUARE_OUTLINE_THICKNESS;
        location1.y = location1.y + SQUARE_SIDE_LENGTH + 3 * SQUARE_OUTLINE_THICKNESS;
        rectangle4->setPosition(location1);

        shapeCenter.y = location1.y;
        shapeCenter.x = location1.x;
    }
};
