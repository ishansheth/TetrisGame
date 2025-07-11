#include "ShapeGenerator.h"
#include "LShape.h"
#include "SShape.h"
#include "SqShape.h"
#include "StShape.h"
#include "TShape.h"
#include "BombShape.h"

static constexpr unsigned int arr_len = 10;
static constexpr unsigned int testShapes[arr_len] = {4,4,4,4,4,4,4,4,4,2};
static unsigned int idx = 0;

IShape* ShapeGenerator::getNextShape (sf::Vector2f position, DisplayContainer* dCont) {
    auto shapeNumber = shapeIndexArray[1];

    if (shapeNumber == 1) {
        // s shape
        SShape* a = new SShape (dCont);
        a->setposition (position);
        return a;
    } else if (shapeNumber == 2) {
        // L shape
        LShape* a = new LShape (dCont);
        a->setposition (position);
        return a;
    } else if (shapeNumber == 3) {
        // | shape
        StShape* a = new StShape (dCont);
        a->setposition (position);
        return a;
    } else if (shapeNumber == 4) {
        // o shape
        SqShape* a = new SqShape (dCont);
        a->setposition (position);
        return a;
    } else if (shapeNumber == 5) {
        // T shape
        TShape* a = new TShape (dCont);
        a->setposition (position);
        return a;
    } else if (shapeNumber == 6) {
        // bomb shape
        BombShape* a = new BombShape (dCont);
        a->setposition (position);
        return a;
    }

}

IShape* ShapeGenerator::getShape (sf::Vector2f position, DisplayContainer* dCont) {
    auto shapeNumber = shapeIndexArray[0];

    if (shapeNumber == 1) {
        // s shape
        SShape* a = new SShape (dCont);
        a->setposition (position);
        return a;
    } else if (shapeNumber == 2) {
        // L shape
        LShape* a = new LShape (dCont);
        a->setposition (position);
        return a;
    } else if (shapeNumber == 3) {
        // | shape
        StShape* a = new StShape (dCont);
        a->setposition (position);
        return a;
    } else if (shapeNumber == 4) {
        // o shape
        SqShape* a = new SqShape (dCont);
        a->setposition (position);
        return a;
    } else if (shapeNumber == 5) {
        // T shape
        TShape* a = new TShape (dCont);
        a->setposition (position);
        return a;
    } else if (shapeNumber == 6) {
        // bomb shape
        BombShape* a = new BombShape (dCont);
        a->setposition (position);
        return a;
    }

}

void ShapeGenerator::generateShapes () {
    auto shapeNumber   = uniformDistribution (rng);

        // shapeIndexArray[0] = shapeIndexArray[1];
        // shapeIndexArray[1] = shapeNumber;

        shapeIndexArray[0] = 2;
        shapeIndexArray[1] = 2;

}

void ShapeGenerator::setAllowedShapesCount(unsigned int cnt)
{
    uniformDistribution.param(std::uniform_int_distribution<std::mt19937::result_type>::param_type{1, cnt});
}
