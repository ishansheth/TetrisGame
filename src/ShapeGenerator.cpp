#include "ShapeGenerator.h"
#include "BombShape.h"
#include "LShape.h"
#include "SShape.h"
#include "SqShape.h"
#include "StShape.h"
#include "TShape.h"
#include "SingleSquareShape.h"

static unsigned int idx = 0;

static constexpr unsigned int arr_len = 14;
static constexpr unsigned int testShapes[arr_len] = {5, 5, 5, 1, 1, 2, 2, 2, 2, 2, 2, 2, 6, 6};

IShape* ShapeGenerator::getNumberMappedShape(unsigned int shapeNumber, sf::Vector2f position, DisplayContainer *dCont)
{
    if (shapeNumber == 1)
    {
        // s shape
        SShape *shape = new SShape(dCont);
        shape->setPosition(position);
        return shape;
    }
    else if (shapeNumber == 2)
    {
        // L shape
        LShape *shape = new LShape(dCont);
        shape->setPosition(position);
        return shape;
    }
    else if (shapeNumber == 3)
    {
        // | shape
        StShape *shape = new StShape(dCont);
        shape->setPosition(position);
        return shape;
    }
    else if (shapeNumber == 4)
    {
        // o shape
        SqShape *shape = new SqShape(dCont);
        shape->setPosition(position);
        return shape;
    }
    else if (shapeNumber == 5)
    {
        // T shape
        TShape *shape = new TShape(dCont);
        shape->setPosition(position);
        return shape;
    }
    else if (shapeNumber == 6)
    {
        // bomb shape
        BombShape *shape = new BombShape(dCont);
        shape->setPosition(position);
        return shape;
    }
    else
    {
        // s shape
        SShape *defaultShape = new SShape(dCont);
        defaultShape->setPosition(position);
        return defaultShape;
    }
}

IShape* ShapeGenerator::getNextShape(sf::Vector2f position, DisplayContainer *dCont)
{
    return getNumberMappedShape(shapeIndexArray[1], position, dCont);
}

IShape *ShapeGenerator::getShape(sf::Vector2f position, DisplayContainer *dCont)
{
    return getNumberMappedShape(shapeIndexArray[0], position, dCont);
}

void ShapeGenerator::generateShapes()
{
    // if (idx < arr_len - 1)
    // {
    //     shapeIndexArray[0] = testShapes[idx];
    //     shapeIndexArray[1] = testShapes[idx + 1];
    //     idx += 1;
    // }
    // else
    {
        auto shapeNumber = uniformDistribution(rng);

        shapeIndexArray[0] = shapeIndexArray[1];
        shapeIndexArray[1] = shapeNumber;

        // shapeIndexArray[0] = 2;
        // shapeIndexArray[1] = 2;
    }
}

IShape* ShapeGenerator::getNewAddedShape(sf::Vector2f position, DisplayContainer *dCont)
{
    return getNumberMappedShape(lastAllowedShape, position, dCont);
}

void ShapeGenerator::setAllowedShapesCount(unsigned int cnt)
{
    uniformDistribution.param(std::uniform_int_distribution<std::mt19937::result_type>::param_type{1, cnt});
    lastAllowedShape = cnt;
}


IShape* ShapeGenerator::getSingleSquareShape(unsigned int idx, unsigned int ypos)
{
    auto xpos = idx*SQUARE_SIDE_LENGTH_WITH_OUTLINE+SQUARE_OUTLINE_THICKNESS;
    IShape* shape = new SingleSquare(sf::Color(129, 133, 137));
    shape->setPosition(sf::Vector2f{xpos,ypos});
    return shape;
}
