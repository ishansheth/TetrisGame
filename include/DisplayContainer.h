#pragma once
#include "FontContainer.h"
#include "GameConstants.h"
#include "IShape.h"
#include "ShapeGenerator.h"
#include <SFML/Graphics.hpp>
#include <random>
#include <set>
#include <unordered_map>
#include <iostream>

static const sf::Vertex partitionLine[] = { sf::Vertex (sf::Vector2f (DRAW_WINDOW_WIDTH, 1.f)),
    sf::Vertex (sf::Vector2f (DRAW_WINDOW_WIDTH, WINDOW_HEIGHT-0.1f)) };

static std::vector<std::vector<sf::Vertex>> getGridLines () {
    static std::vector<std::vector<sf::Vertex>> gridLines;
    for (std::size_t i = 1; i < 20; i++) {
        std::vector<sf::Vertex> singleLine{
            sf::Vertex (sf::Vector2f ((SQUARE_SIDE_LENGTH_WITH_OUTLINE * i), 1.f), sf::Color (255, 0, 0, 100)),
            sf::Vertex (sf::Vector2f ((SQUARE_SIDE_LENGTH_WITH_OUTLINE * i), WINDOW_HEIGHT-0.1f), sf::Color (255, 0, 0, 100))
        };

        gridLines.push_back (singleLine);
    }
}

static const sf::Vertex borderLine1[] = { sf::Vertex (sf::Vector2f (1.f, 1.f)),
    sf::Vertex (sf::Vector2f (WINDOW_WIDTH-0.1f, 1.f)) };

static const sf::Vertex borderLine2[] = { sf::Vertex (sf::Vector2f (WINDOW_WIDTH-0.1f, 1.f)),
    sf::Vertex (sf::Vector2f (WINDOW_WIDTH-0.1f, WINDOW_HEIGHT-0.1f)) };

static const sf::Vertex borderLine3[] = { sf::Vertex (sf::Vector2f (WINDOW_WIDTH-0.1f, WINDOW_HEIGHT-0.1f)),
    sf::Vertex (sf::Vector2f (1.f, WINDOW_HEIGHT-0.1f)) };

static const sf::Vertex borderLine4[] = { sf::Vertex (sf::Vector2f (1.f, WINDOW_HEIGHT-0.1f)),
    sf::Vertex (sf::Vector2f (1.f, 1.f)) };

class DisplayContainer {
    std::vector<IShape*> mDisplayContainer;
    std::vector<int> rowYCoordinate;
    FontContainer& fContainerRef;
    ShapeGenerator& shapeGen;

    IShape* lastShape;
    IShape* nextShape;
    std::map<int, std::vector<std::pair<sf::RectangleShape*, IShape*>>> individualComponentContainer;
    std::vector<int> yPositions;
    bool moveStatus;
    std::map<int, int> blockCountInRow;
    int scoreValue;
    bool isGameOverState;

    public:
    DisplayContainer (FontContainer& fCon, ShapeGenerator& shapegenerator)
    : shapeGen(shapegenerator), lastShape (nullptr), nextShape (nullptr), moveStatus (true),
      scoreValue (0), isGameOverState (false), fContainerRef (fCon) {

        auto yVal = LAST_ROW_Y;
        for (int i = NUMBER_OF_ROWS_IN_GAME; i > 0; i--) {
            std::cout<<"row y values:"<<yVal<<std::endl;
            individualComponentContainer[yVal] =
            std::vector<std::pair<sf::RectangleShape*, IShape*>> ();
            rowYCoordinate.push_back(yVal);
            yVal -= SQUARE_SIDE_LENGTH_WITH_OUTLINE;
        }
    }

    void generateAndDrawShape (sf::RenderWindow&);

    void processshapes ();

    void handleKey (sf::Keyboard::Key k);

    void drawShape (sf::RenderWindow& displayWindow);

    void drawNextShape (IShape* shape, sf::RenderWindow& displayWindow);

    // check of falling shape is intersecting with currently displayed shapes
    bool isIntersecting (sf::Vector2f shapePosition);

    int getLowestYVal (int x, int refY);

    void shiftStructureDownward ();

    void checkFullRows ();

    int getScore ();

    bool isGameOver ();

    void handleGameState (sf::RenderWindow& displayWindow);

    int getAllowedYVal(float yCoordinate);

};
