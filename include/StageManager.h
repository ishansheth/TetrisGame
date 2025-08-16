#pragma once
#include "DisplayContainer.h"
#include "IShape.h"

class StageManager
{
    unsigned int currentStageNumber;
    ShapeGenerator shapeGenerator;

    DisplayContainer disPlayContainer;
    sf::RenderWindow &displayWindow;
    std::vector<unsigned int> winScoreForStage;
    void setStageParameters();

    std::vector<IShape *> getAllowedShapedOfStage();

    void setWinningScoreForstage();

  public:
    StageManager(FontContainer &fCon, sf::RenderWindow &window);

    void checkStageStatus();

    void generateStageScreen();

    void handleUserInput(sf::Keyboard::Key);

    void processStageShapes();
};