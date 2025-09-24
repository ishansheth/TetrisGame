#pragma once
#include "FontContainer.h"
#include "GameConstants.h"
#include "IShape.h"
#include "ParticleSystem.h"
#include "ShapeGenerator.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <set>
#include <unordered_map>
#include <fstream>
#include <cstdint>

static const sf::Vertex partitionLine[] = {sf::Vertex(sf::Vector2f(DRAW_WINDOW_WIDTH, 1.f)),
                                           sf::Vertex(sf::Vector2f(DRAW_WINDOW_WIDTH, WINDOW_HEIGHT - 0.1f))};

static const sf::Vertex borderLine1[] = {sf::Vertex(sf::Vector2f(1.f, 1.f)),
                                         sf::Vertex(sf::Vector2f(WINDOW_WIDTH - 0.1f, 1.f))};

static const sf::Vertex borderLine2[] = {sf::Vertex(sf::Vector2f(WINDOW_WIDTH - 0.1f, 1.f)),
                                         sf::Vertex(sf::Vector2f(WINDOW_WIDTH - 0.1f, WINDOW_HEIGHT - 0.1f))};

static const sf::Vertex borderLine3[] = {sf::Vertex(sf::Vector2f(WINDOW_WIDTH - 0.1f, WINDOW_HEIGHT - 0.1f)),
                                         sf::Vertex(sf::Vector2f(1.f, WINDOW_HEIGHT - 0.1f))};

static const sf::Vertex borderLine4[] = {sf::Vertex(sf::Vector2f(1.f, WINDOW_HEIGHT - 0.1f)),
                                         sf::Vertex(sf::Vector2f(1.f, 1.f))};

static const sf::Vertex highscore_partition_line1[] = {sf::Vertex(sf::Vector2f(DRAW_WINDOW_WIDTH, 240)),
                                         sf::Vertex(sf::Vector2f(WINDOW_WIDTH, 240))};

static const sf::Vertex highscore_partition_line2[] = {sf::Vertex(sf::Vector2f(DRAW_WINDOW_WIDTH, 242)),
                                        sf::Vertex(sf::Vector2f(WINDOW_WIDTH, 242))};



class DisplayContainer
{
    std::vector<unsigned int> rowYCoordinate;
    std::vector<unsigned int> rowXCoordinate;

    std::map<int, std::vector<std::pair<sf::RectangleShape **, IShape *>>> individualComponentContainer;
    std::vector<int> yPositions;

    sf::SoundBuffer shapeSettleSoundBuffer;
    sf::Sound shapeSettleSound;

    sf::SoundBuffer rowRemovedExplosionSoundBuffer;
    sf::Sound rowRemovedExplosionSound;

    sf::SoundBuffer bombExplosionSoundBuffer;
    sf::Sound bombExplosionSound;

    std::vector<ParticleSystem> rowCollapseParticleSystems;
    ParticleSystem bombExplosionParticles;

    sf::Clock rowInsertionTimer;
    sf::Time oneMinTime;

    FontContainer &fContainerRef;
    ShapeGenerator &shapeGen;

    IShape *lastShape;
    IShape *nextShape;

    bool moveStatus;
    int scoreValue;
    unsigned int currentStageNumber;
    
    // state variables
    bool isGameOverState;
    bool isGamePaused;
    bool displayEnterUsernameScreen;
    bool highScoreAchieved;
    bool insertRowsAtbottom;
    bool gameComplete;
    bool windowClosePressed;
    bool terminateBomb;

    std::string highScoreUsername;
    uint32_t currentscore;
    std::string highScoreDisplayData;
    
    int getLowestYVal(const float x, const float refY);

    void terminateBombEarly(sf::RenderWindow &displayWindow);

    void shiftStructureDownward(sf::RenderWindow &displayWindow, unsigned int yVal);

    void checkFullRows(sf::RenderWindow &displayWindow);

    void setParamtersForCurrentStage();

    void drawShape(sf::RenderWindow &displayWindow);

    bool isGameOver();

    unsigned int getAllowedYVal(const float yCoordinate);

    unsigned int getAllowedXVal(const float xCoordinate);

    void moveShapes();

    void handleBombDrop(sf::RenderWindow &displayWindow);

    void drawDisplayContainer(sf::RenderWindow &displayWindow);

    void insertRowAtBottom();

    void showGameCompleteScreenAndExit(sf::RenderWindow &displayWindow);

    void makeRowFall(unsigned int sourceY, sf::RenderWindow &displayWindow);

    void createBombDamage(std::set<IShape*> brokenShapes, sf::RenderWindow& displayWindow);

    void highlightBombDestroyedShapes();

    void getBombDestructionBox(unsigned int& minX, unsigned int& upperMostY);

  public:
    DisplayContainer(FontContainer &fCon, ShapeGenerator &shapegenerator);

    void setGamePaused();

    void resetGamePaused();

    bool isIntersecting(const sf::Vector2f &shapePosition, const IShape *ignoreshape);

    void generateAndDrawShape(sf::RenderWindow &);

    void processshapes(sf::RenderWindow &displayWindow);

    void handleKey(const sf::Keyboard::Key &k);

    void handleGameState(sf::RenderWindow &displayWindow);

    void showCurrentStageScreen(sf::RenderWindow &displayWindow);

    void cleanDisplayContainer();

    void prepareDefaultScreenItems(sf::RenderWindow &displayWindow);

    void eraseCompletedRow(int removedRowY, sf::RenderWindow &displayWindow);

    void saveHighScoreInFile();

    void handleWindowCloseEvent();

};
