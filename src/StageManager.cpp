#include "StageManager.h"

StageManager::StageManager (FontContainer& fCon, sf::RenderWindow& window) : 
        shapeGenerator(),
        currentStageNumber(1), 
        displayWindow(window),
        disPlayContainer(fCon,shapeGenerator)
{
    winScoreForStage.push_back(50);
    winScoreForStage.push_back(150);
    winScoreForStage.push_back(500);
    winScoreForStage.push_back(1000);
}

void StageManager::checkStageStatus()
{
}

void StageManager::generateStageScreen ()
{

}

void StageManager::handleUserInput(sf::Keyboard::Key pressedKey)
{

}

void StageManager::processStageShapes()
{

}
