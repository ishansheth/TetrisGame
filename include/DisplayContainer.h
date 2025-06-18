#pragma once
#include "FontContainer.h"
#include "GameConstants.h"
#include "IShape.h"
#include "ShapeGenerator.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <random>
#include <set>
#include <unordered_map>
#include <iostream>

static const sf::Vertex partitionLine[] = { sf::Vertex (sf::Vector2f (DRAW_WINDOW_WIDTH, 1.f)),
    sf::Vertex (sf::Vector2f (DRAW_WINDOW_WIDTH, WINDOW_HEIGHT-0.1f)) };

static std::vector<std::vector<sf::Vertex>> getGridLines () {
    static std::vector<std::vector<sf::Vertex>> gridLines;
    for (std::size_t i = 1; i < NUMBER_OF_SQUARES_IN_ROW; i++) {
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

    std::vector<int> rowYCoordinate;
    std::map<int, std::vector<std::pair<sf::RectangleShape*, IShape*>>> individualComponentContainer;
    std::vector<int> yPositions;
    std::vector<unsigned int> winScoreForStage;

    sf::SoundBuffer shapeSettleSoundBuffer;
    sf::Sound shapeSettleSound;
    
    FontContainer& fContainerRef;
    ShapeGenerator& shapeGen;

    IShape* lastShape;
    IShape* nextShape;

    bool moveStatus;
    int scoreValue;
    unsigned int currentStageNumber;

    // state variables
    bool isGameOverState;
    bool isGamePaused;

    int getLowestYVal (int x, int refY);

    void shiftStructureDownward ();

    void checkFullRows ();

    void setParamtersForCurrentStage();

    void drawShape (sf::RenderWindow& displayWindow);

    int getScore ();

    bool isGameOver ();

    int getAllowedYVal(float yCoordinate);

    void moveShapes();

    public:
    DisplayContainer (FontContainer& fCon, ShapeGenerator& shapegenerator);

    void setGamePaused();

    void resetGamePaused();

    bool isIntersecting (sf::Vector2f shapePosition);

    void generateAndDrawShape (sf::RenderWindow&);

    void processshapes ();

    void handleKey (sf::Keyboard::Key k);

    void handleGameState (sf::RenderWindow& displayWindow);

    void showCurrentStageScreen(sf::RenderWindow& displayWindow);

};
