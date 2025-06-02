#pragma once
#include <IShape.h>
#include <SFML/Graphics.hpp>
#include <random>

class DisplayContainer;

class ShapeGenerator {
    // index 0 current shape
    // index 1 next shape
    int shapeIndexArray[2];
    std::random_device dev;
    std::mt19937 rng;
    std::uniform_int_distribution<std::mt19937::result_type> dist;

    public:
    ShapeGenerator () : shapeIndexArray{ -1, -1 }, rng (dev ()), dist (1, 5) {
        shapeIndexArray[0] = dist (rng);
        shapeIndexArray[1] = dist (rng);
    }

    IShape* getNextShape (sf::Vector2f position, DisplayContainer* dCont);

    IShape* getShape (sf::Vector2f position, DisplayContainer* dCont);

    void generateShapes ();
};
