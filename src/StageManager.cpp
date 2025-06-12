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
    // set allowed shapes in shape generator at the beginning of every stage
    if(disPlayContainer.getScore() >= winScoreForStage[currentStageNumber])
    {
        if(currentStageNumber < winScoreForStage.size()-1)
        {
            currentStageNumber++;            
        }
        else
        {
            // decide what to do here?? all stages are cleared
        }
        
        // remove all the shapes from container
        // set the stage paramters
        // create new allopwed shapes
        // generate allowed shapes for the stage
        if(currentStageNumber == 0)
        {
        }
        else if(currentStageNumber == 1)
        {            
        }
        else if(currentStageNumber == 2)
        {            
        }
        else if(currentStageNumber == 4)
        {            
        }

    }
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
