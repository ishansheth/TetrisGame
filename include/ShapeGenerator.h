#pragma once
#include <IShape.h>
#include <SFML/Graphics.hpp>
#include <random>
#include <vector>

class ShapeGenerator
{
    // index 0 current shape
    // index 1 next shape
    int shapeIndexArray[2];
    std::random_device dev;
    std::mt19937 rng;
    std::uniform_int_distribution<std::mt19937::result_type> uniformDistribution;

  public:
    ShapeGenerator() : shapeIndexArray{-1, -1}, rng(dev()), uniformDistribution(1, 5)
    {
        shapeIndexArray[0] = uniformDistribution(rng);
        shapeIndexArray[1] = uniformDistribution(rng);
    }

    IShape *getNextShape(sf::Vector2f position, DisplayContainer *dCont);

    IShape *getShape(sf::Vector2f position, DisplayContainer *dCont);

    void generateShapes();

    // sets the allowed shapes to be generated from
    // if you call again with different input container, last one will be replaced
    void setAllowedShapesCount(unsigned int);

    IShape* getSingleSquareShape(unsigned int idx, unsigned int ypos);
};
