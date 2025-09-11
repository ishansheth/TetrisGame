#pragma once
#include "BaseShape.h"
#include "DisplayContainer.h"
#include "Util.h"
#include <SFML/Graphics.hpp>

struct TShape : public BaseShape
{
    // T shape
    // | 1 |
    // | 2 | 4 |
    // | 3 |

  public:
    TShape(DisplayContainer *displayManager) : BaseShape(displayManager, sf::Color(213, 197, 138))
    {
    }

    virtual ~TShape()
    {
    }

    virtual bool isBomb() const override
    {
        return false;
    }

    virtual void setPosition(sf::Vector2f location) override
    {
        auto location1 = location;
        location1.x = location1.x + SQUARE_OUTLINE_THICKNESS;
        location1.y = location1.y + SQUARE_OUTLINE_THICKNESS;
        rectangle1->setPosition(location1);

        location1 = location;
        location1.y = location1.y + (SQUARE_SIDE_LENGTH) + 3 * SQUARE_OUTLINE_THICKNESS;
        location1.x = location1.x + SQUARE_OUTLINE_THICKNESS;
        rectangle2->setPosition(location1);

        location1 = location;
        location1.y = location1.y + (2 * SQUARE_SIDE_LENGTH) + 5 * SQUARE_OUTLINE_THICKNESS;
        location1.x = location1.x + SQUARE_OUTLINE_THICKNESS;
        rectangle3->setPosition(location1);

        // shapecenter is at 3rd rectangle upper right corner
        shapeCenter.y = location1.y;
        shapeCenter.x = location1.x;

        location1 = location;
        location1.y = location1.y + (SQUARE_SIDE_LENGTH) + 3 * SQUARE_OUTLINE_THICKNESS;
        location1.x = location1.x + (SQUARE_SIDE_LENGTH) + 3 * SQUARE_OUTLINE_THICKNESS;
        rectangle4->setPosition(location1);
    }
};
