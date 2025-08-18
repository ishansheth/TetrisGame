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
static constexpr unsigned int testShapes[arr_len] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

IShape *ShapeGenerator::getNextShape(sf::Vector2f position, DisplayContainer *dCont)
{
    auto shapeNumber = shapeIndexArray[1];

    if (shapeNumber == 1)
    {
        // s shape
        SShape *a = new SShape(dCont);
        a->setposition(position);
        return a;
    }
    else if (shapeNumber == 2)
    {
        // L shape
        LShape *a = new LShape(dCont);
        a->setposition(position);
        return a;
    }
    else if (shapeNumber == 3)
    {
        // | shape
        StShape *a = new StShape(dCont);
        a->setposition(position);
        return a;
    }
    else if (shapeNumber == 4)
    {
        // o shape
        SqShape *a = new SqShape(dCont);
        a->setposition(position);
        return a;
    }
    else if (shapeNumber == 5)
    {
        // T shape
        TShape *a = new TShape(dCont);
        a->setposition(position);
        return a;
    }
    else if (shapeNumber == 6)
    {
        // bomb shape
        BombShape *a = new BombShape(dCont);
        a->setposition(position);
        return a;
    }
}

IShape *ShapeGenerator::getShape(sf::Vector2f position, DisplayContainer *dCont)
{
    auto shapeNumber = shapeIndexArray[0];

    if (shapeNumber == 1)
    {
        // s shape
        SShape *a = new SShape(dCont);
        a->setposition(position);
        return a;
    }
    else if (shapeNumber == 2)
    {
        // L shape
        LShape *a = new LShape(dCont);
        a->setposition(position);
        return a;
    }
    else if (shapeNumber == 3)
    {
        // | shape
        StShape *a = new StShape(dCont);
        a->setposition(position);
        return a;
    }
    else if (shapeNumber == 4)
    {
        // o shape
        SqShape *a = new SqShape(dCont);
        a->setposition(position);
        return a;
    }
    else if (shapeNumber == 5)
    {
        // T shape
        TShape *a = new TShape(dCont);
        a->setposition(position);
        return a;
    }
    else if (shapeNumber == 6)
    {
        // bomb shape
        BombShape *a = new BombShape(dCont);
        a->setposition(position);
        return a;
    }
}

void ShapeGenerator::generateShapes()
{
    if (idx < arr_len - 1)
    {
        shapeIndexArray[0] = testShapes[idx];
        shapeIndexArray[1] = testShapes[idx + 1];
        idx += 1;
    }
    else
    {
        auto shapeNumber = uniformDistribution(rng);

        shapeIndexArray[0] = shapeIndexArray[1];
        shapeIndexArray[1] = shapeNumber;

        // shapeIndexArray[0] = 2;
        // shapeIndexArray[1] = 2;
    }
}

void ShapeGenerator::setAllowedShapesCount(unsigned int cnt)
{
    uniformDistribution.param(std::uniform_int_distribution<std::mt19937::result_type>::param_type{1, cnt});
}


IShape* ShapeGenerator::getSingleSquareShape(unsigned int idx, unsigned int ypos)
{
    auto xpos = idx*SQUARE_SIDE_LENGTH_WITH_OUTLINE+SQUARE_OUTLINE_THICKNESS;
    IShape* shape = new SingleSquare(sf::Color(129, 133, 137));
    shape->setposition(sf::Vector2f{xpos,ypos});
    return shape;
}
