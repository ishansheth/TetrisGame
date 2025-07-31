#pragma once
#include "BaseShape.h"
#include "DisplayContainer.h"
#include "Util.h"
#include <SFML/Graphics.hpp>

struct StShape : public BaseShape
{
    // stick shape

  public:
    StShape(DisplayContainer *displayManager) : BaseShape(displayManager, sf::Color(250, 50, 100))
    {
    }

    IShape *clone(DisplayContainer *displayManager) override
    {
        StShape *clonedObj = new StShape(displayManager);
        return clonedObj;
    }

    virtual ~StShape()
    {
    }

    virtual bool isBomb() override
    {
        return false;
    }

    virtual void setposition(sf::Vector2f location) override
    {
        auto location1 = location;
        location1.x = location1.x + SQUARE_OUTLINE_THICKNESS;
        location1.y = location1.y + SQUARE_OUTLINE_THICKNESS;
        rectangle1->setPosition(location1);

        location1 = location;
        location1.x = location1.x + (SQUARE_SIDE_LENGTH) + 3 * SQUARE_OUTLINE_THICKNESS;
        location1.y = location1.y + SQUARE_OUTLINE_THICKNESS;
        rectangle2->setPosition(location1);

        shapeCenter.x = location1.x;
        shapeCenter.y = location1.y;

        location1 = location;
        location1.x = location1.x + 2 * (SQUARE_SIDE_LENGTH) + 5 * SQUARE_OUTLINE_THICKNESS;
        location1.y = location1.y + SQUARE_OUTLINE_THICKNESS;
        rectangle3->setPosition(location1);

        location1 = location;
        location1.x = location1.x + 3 * (SQUARE_SIDE_LENGTH) + 7 * SQUARE_OUTLINE_THICKNESS;
        location1.y = location1.y + SQUARE_OUTLINE_THICKNESS;
        rectangle4->setPosition(location1);
    }
};
