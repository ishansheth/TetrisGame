#pragma once
#include "FontContainer.h"
#include "GameConstants.h"
#include "IShape.h"
#include "ShapeGenerator.h"
#include <SFML/Graphics.hpp>
#include <random>
#include <set>
#include <unordered_map>


static const sf::Vertex partitionLine[] = { sf::Vertex (sf::Vector2f (520.f, 1.f)),
    sf::Vertex (sf::Vector2f (520.f, 599.f)) };

static std::vector<std::vector<sf::Vertex>> getGridLines () {
    static std::vector<std::vector<sf::Vertex>> gridLines;
    for (std::size_t i = 1; i < 20; i++) {
        std::vector<sf::Vertex> singleLine{
            sf::Vertex (sf::Vector2f ((26.f * i), 1.f), sf::Color (255, 0, 0, 100)),
            sf::Vertex (sf::Vector2f ((26.f * i), 599.f), sf::Color (255, 0, 0, 100))
        };

        gridLines.push_back (singleLine);
    }
}

static const sf::Vertex borderLine1[] = { sf::Vertex (sf::Vector2f (1.f, 1.f)),
    sf::Vertex (sf::Vector2f (799.f, 1.f)) };

static const sf::Vertex borderLine2[] = { sf::Vertex (sf::Vector2f (799.f, 1.f)),
    sf::Vertex (sf::Vector2f (799.f, 599.f)) };

static const sf::Vertex borderLine3[] = { sf::Vertex (sf::Vector2f (799.f, 599.f)),
    sf::Vertex (sf::Vector2f (1.f, 599.f)) };

static const sf::Vertex borderLine4[] = { sf::Vertex (sf::Vector2f (1.f, 599.f)),
    sf::Vertex (sf::Vector2f (1.f, 1.f)) };

class DisplayContainer {
    std::vector<IShape*> mDisplayContainer;
    FontContainer& fContainerRef;
    ShapeGenerator shapeGen;

    IShape* lastShape;
    IShape* nextShape;
    std::map<int, std::vector<std::pair<sf::RectangleShape*, IShape*>>> individualComponentContainer;
    std::vector<int> yPositions;
    bool moveStatus;
    std::map<int, int> blockCountInRow;
    int scoreValue;
    bool isGameOverState;

    public:
    DisplayContainer (FontContainer& fCon)
    : lastShape (nullptr), nextShape (nullptr), moveStatus (true),
      scoreValue (0), isGameOverState (false), fContainerRef (fCon) {
        for (int i = FIRST_ROW_Y; i < WINDOW_HEIGHT;
             i     = i + (SQUARE_SIDE_LENGTH + 2 * SQUARE_OUTLINE_THICKNESS)) {
            individualComponentContainer[i] =
            std::vector<std::pair<sf::RectangleShape*, IShape*>> ();
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
};
