#include "DisplayContainer.h"
#include <iostream>
#include <thread>

DisplayContainer::DisplayContainer (FontContainer& fCon, ShapeGenerator& shapegenerator)
: shapeGen(shapegenerator), lastShape (nullptr), nextShape (nullptr), moveStatus (true),
    scoreValue (0), isGameOverState (false),isGamePaused(false), fContainerRef (fCon), currentStageNumber(1) {

    auto yVal = LAST_ROW_Y;
    for (int i = NUMBER_OF_ROWS_IN_GAME; i > 0; i--) {
        std::cout<<"row y values:"<<yVal<<std::endl;
        individualComponentContainer[yVal] =
        std::vector<std::pair<sf::RectangleShape*, IShape*>> ();
        rowYCoordinate.push_back(yVal);
        yVal -= SQUARE_SIDE_LENGTH_WITH_OUTLINE;
    }

    winScoreForStage.push_back(20);
    winScoreForStage.push_back(150);
    winScoreForStage.push_back(500);
    winScoreForStage.push_back(1000);

    setParamtersForCurrentStage();
}


bool DisplayContainer::isGameOver () {
    return (individualComponentContainer[FIRST_ROW_Y].size () > 0);
}

int DisplayContainer::getAllowedYVal(float yCoordinate)
{
    int minIdx = -1;
    int minDiff = 1000;

    for(std::size_t i = 0; i < rowYCoordinate.size(); i++)
    {
        if(abs(rowYCoordinate[i]-yCoordinate) < minDiff)
        {
            minDiff = abs(rowYCoordinate[i]-yCoordinate);
            minIdx = i;
        }
    }
    return rowYCoordinate[minIdx];
}

int DisplayContainer::getScore () {
    return scoreValue;
}

void DisplayContainer::handleKey (sf::Keyboard::Key k) {
    if(lastShape)
        lastShape->handleKey (k);
}

void DisplayContainer::generateAndDrawShape (sf::RenderWindow& displayWindow) {
    if (isGameOverState) {
        return;
    }

    if (lastShape == nullptr) {
        shapeGen.generateShapes ();
        lastShape = shapeGen.getShape (sf::Vector2f (0, 0), this);
        nextShape = shapeGen.getNextShape (sf::Vector2f (NEXT_SHAPE_X, NEXT_SHAPE_Y), this);
    }

    for (auto& s : individualComponentContainer) {
        for (auto& element : s.second) {
            displayWindow.draw (*(element.first));
        }
    }
    lastShape->drawShape (displayWindow);
    nextShape->drawShape (displayWindow);

    std::string displayScoreVal = std::to_string (getScore ());
    fContainerRef.setFontString (GameFontStrings::SCORE_VALUE, displayScoreVal);

    std::string stageVal = std::to_string (currentStageNumber);
    fContainerRef.setFontString (GameFontStrings::STAGE_VALUE, stageVal);
}

void DisplayContainer::processshapes () {
    if (isGameOverState) {
        return;
    }

    if(lastShape == nullptr)
    {
        return;
    }

    moveStatus = lastShape->getMoveStatus ();
    if (!moveStatus) {
        // mapping individual component of shape to its y co-ordinate
        for (auto& s : lastShape->getShapeContianer ()) {
            auto yVal = getAllowedYVal(s->getPosition ().y);
            auto lowerBoundPtr = individualComponentContainer.lower_bound (yVal);

            if (lowerBoundPtr != individualComponentContainer.end ()) {
                lowerBoundPtr->second.push_back (std::make_pair (s, lastShape));
            }
        }

        checkFullRows ();
        lastShape = nullptr;
    }

}

// void DisplayContainer::drawShape (sf::RenderWindow& displayWindow) {
//     for (auto& s : individualComponentContainer) {
//         for (auto& element : s.second) {
//             displayWindow.draw (*(element.first));
//         }
//     }
//     lastShape->drawShape (displayWindow);
// }

// void DisplayContainer::drawNextShape (IShape* shape, sf::RenderWindow& displayWindow) {
//     shape->drawAsNextShape (displayWindow);
// }

// check of falling shape is intersecting with currently displayed shapes
bool DisplayContainer::isIntersecting (sf::Vector2f shapePosition) {
    // get all the shapes for the container which is currently displayed in window
    for (auto& s : individualComponentContainer) {
        // for incoming shape, iterate over all rectangles
        // check the shape in container
        for (auto& t2 : s.second) {
            if (
            //                    bottom >= top
            shapePosition.y + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS >=
            t2.first->getPosition ().y - SQUARE_OUTLINE_THICKNESS &&
            //                    top <= bottom
            shapePosition.y <= t2.first->getPosition ().y + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS &&
            //                    left <= right
            shapePosition.x <= t2.first->getPosition ().x + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS &&
            //                    right >= left
            shapePosition.x + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS >=
            t2.first->getPosition ().x)

            {

                return true;
            }
        }
    }
    return false;
}

int DisplayContainer::getLowestYVal (int x, int refY) {
    for (auto it = individualComponentContainer.begin ();
         it != individualComponentContainer.end (); it++) {
        if (it->first > refY) {
            bool found = false;
            for (auto& element : it->second) {
                if (abs(element.first->getPosition ().x - x) <= 3) {
                    it--;
                    std::cout<<"x:"<<x<<"-->return:"<<it->first<<std::endl;
                    return it->first;
                }
            }
        }
    }
    std::cout<<"not found: x:"<<x<<"-->return:"<<rowYCoordinate.front()<<std::endl;
    return rowYCoordinate.front();
}

void DisplayContainer::shiftStructureDownward () {
    bool shiftRequired = false;
    int startShiftYVal = 0;
    int fullRowYVal    = 0;
    for (auto it = individualComponentContainer.rbegin ();
         it != individualComponentContainer.rend (); it++) {
        if (it->second.size () == NUMBER_OF_SQUARES_IN_ROW) {
            std::cout<<"Removing row at:"<<it->first<<std::endl;
            shiftRequired = true;
            if (fullRowYVal == 0) {
                fullRowYVal = it->first;
            }
            for (auto& fRC : it->second) {
                delete fRC.first;
                fRC.first = nullptr;
                fRC.second->setBroken();
            }
            it->second.clear();
            std::cout<<"cleared:"<<it->first<<std::endl;
            scoreValue += SCORE_PER_ROW;
            it++;
            startShiftYVal = it->first;
            std::cout<<"start shifting from:"<<startShiftYVal<<"  to:"<<fullRowYVal<<std::endl;
            break;
        }
    }
    std::cout<<"size after clear:"<<individualComponentContainer[fullRowYVal].size()<<std::endl;
    if (shiftRequired) {
        for (auto it = individualComponentContainer.rbegin ();
             it != individualComponentContainer.rend (); it++) {
            if (it->first <= startShiftYVal) {
                for (auto& s : it->second) {
                    auto xVal   = s.first->getPosition ().x;
                    int lowestY = 0;
                    if (s.second->isShapeBroken ()) {
                        lowestY = getLowestYVal (xVal, it->first);
                    } else {
                        lowestY = fullRowYVal;
                    }
                    s.first->setPosition (xVal, lowestY);
                    individualComponentContainer[lowestY].push_back (s);
                }
                individualComponentContainer[startShiftYVal].clear ();
                startShiftYVal -= SQUARE_SIDE_LENGTH_WITH_OUTLINE;
                fullRowYVal -= SQUARE_SIDE_LENGTH_WITH_OUTLINE;
            }
        }
    }
}

void DisplayContainer::checkFullRows () {
    while (1) {
        bool removeRows = false;
        for (auto it = individualComponentContainer.rbegin ();
             it != individualComponentContainer.rend (); it++) {
            std::cout<<it->first<<"---"<<it->second.size()<<std::endl;
            if (it->second.size () == NUMBER_OF_SQUARES_IN_ROW) {
                shiftStructureDownward ();
                removeRows = true;
            }
        }

        if (!removeRows) {
            break;
        }
    }
}

void DisplayContainer::handleGameState (sf::RenderWindow& displayWindow) {
    if (isGameOver ()) {
        isGameOverState = true;
        scoreValue      = 0;
        for (auto it = individualComponentContainer.begin ();
             it != individualComponentContainer.end (); it++) {
            for (auto& element : it->second) {
                if (element.first != nullptr)
                    delete element.first;

                element.second = nullptr;
            }
            it->second.clear ();
        }
        fContainerRef.drawSingleString (displayWindow, GameFontStrings::GAME_OVER);
        sf::Event event;

        while (displayWindow.pollEvent (event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (sf::Keyboard::isKeyPressed (sf::Keyboard::R)) {
                    isGameOverState = false;
                    generateAndDrawShape (displayWindow);
                    break;
                } else if (sf::Keyboard::isKeyPressed (sf::Keyboard::E)) {
                    displayWindow.close ();
                    break;
                }
            }
        }
    }    
    else if(isGamePaused)
    {
        fContainerRef.drawSingleString (displayWindow, GameFontStrings::GAME_PAUSED);
        fContainerRef.drawFonts (displayWindow);
        displayWindow.draw (partitionLine, 2, sf::Lines);
    }
    else if(getScore() >= winScoreForStage[currentStageNumber-1])
    {
        // steps for change stage:
        // show complete stage message
        // wait 1 sec
        // show new stage number message
        // clear the container
        // set the new parameters for the new stage, generate new shapes
        // display
        std::cout<<"============stage complete"<<std::endl;
        // clear the container
        for (auto it = individualComponentContainer.begin ();
             it != individualComponentContainer.end (); it++) {
            for (auto& element : it->second) {
                if (element.first != nullptr)
                    delete element.first;

                element.second = nullptr;
            }
            it->second.clear ();
        }

        // clear the screen
        displayWindow.clear (sf::Color::Black);

        // show stage complete message
        std::string stageCompleteMsg = "Stage " + std::to_string(currentStageNumber) + " Complete!!";
        fContainerRef.setFontString (GameFontStrings::STAGE_COMPLETE_MESSAGE, stageCompleteMsg);
        fContainerRef.drawSingleString (displayWindow, GameFontStrings::STAGE_COMPLETE_MESSAGE);

        // draw border line
        displayWindow.draw (borderLine1, 2, sf::Lines);
        displayWindow.draw (borderLine2, 2, sf::Lines);
        displayWindow.draw (borderLine3, 2, sf::Lines);
        displayWindow.draw (borderLine4, 2, sf::Lines);

        // display the message
        displayWindow.display ();
        // wait 1 sec
        std::this_thread::sleep_for (std::chrono::milliseconds (1000));
        // clear the screen
        displayWindow.clear (sf::Color::Black);

        // draw border line
        displayWindow.draw (borderLine1, 2, sf::Lines);
        displayWindow.draw (borderLine2, 2, sf::Lines);
        displayWindow.draw (borderLine3, 2, sf::Lines);
        displayWindow.draw (borderLine4, 2, sf::Lines);

        // show next stage message
        currentStageNumber++;
        stageCompleteMsg = std::to_string(currentStageNumber);
        // display next stage number message        
        fContainerRef.setFontString (GameFontStrings::STAGE_VALUE, stageCompleteMsg);
        fContainerRef.drawSingleString (displayWindow, GameFontStrings::STAGE_LABEL,STAGE_COMPLETE_MSG_X,STAGE_COMPLETE_MSG_Y);
        fContainerRef.drawSingleString (displayWindow, GameFontStrings::STAGE_VALUE,STAGE_COMPLETE_MSG_X + 80,STAGE_COMPLETE_MSG_Y);
        // display
        displayWindow.display ();
        // wait
        std::this_thread::sleep_for (std::chrono::milliseconds (1000));
        // clear
        displayWindow.clear (sf::Color::Black);

        // reset the position of stage label and values string
        fContainerRef.drawSingleString (displayWindow, GameFontStrings::STAGE_LABEL,STAGE_LEBEL_X, STAGE_LEBEL_Y);
        fContainerRef.drawSingleString (displayWindow, GameFontStrings::STAGE_VALUE,STAGE_VALUE_X, STAGE_VALUE_Y);

        // set the parameter for the next stage

        lastShape = nullptr;
        setParamtersForCurrentStage();
        // display the necessary fonts
        fContainerRef.drawFonts (displayWindow);
        // display partition line
        displayWindow.draw (partitionLine, 2, sf::Lines);

    }
    else if(!isGamePaused && !isGameOverState)
    {
        moveShapes();
        fContainerRef.drawFonts (displayWindow);
        displayWindow.draw (partitionLine, 2, sf::Lines);
    }

    displayWindow.draw (borderLine1, 2, sf::Lines);
    displayWindow.draw (borderLine2, 2, sf::Lines);
    displayWindow.draw (borderLine3, 2, sf::Lines);
    displayWindow.draw (borderLine4, 2, sf::Lines);

    displayWindow.display ();
    displayWindow.clear (sf::Color::Black);
}

void DisplayContainer::moveShapes()
{
    lastShape->moveShape();
}

void DisplayContainer::setGamePaused()
{
    isGamePaused = true;
}

void DisplayContainer::resetGamePaused()
{
    isGamePaused = false;
}


void DisplayContainer::setParamtersForCurrentStage()
{
    if(currentStageNumber == 1)
    {
        SHAPE_DOWN_FALL_SPEED_Y = 0.1f;
        shapeGen.setAllowedShapesCount(SHAPE_COUNT_FOR_STAGE[currentStageNumber-1]);
    }
    else if(currentStageNumber == 2)
    {
        // set allowed shapes
        SHAPE_DOWN_FALL_SPEED_Y = 0.2f;
        shapeGen.setAllowedShapesCount(SHAPE_COUNT_FOR_STAGE[currentStageNumber-1]);
    }    
    else if(currentStageNumber == 3)
    {
        // set allowed shapes
        SHAPE_DOWN_FALL_SPEED_Y = 0.3f;
        shapeGen.setAllowedShapesCount(SHAPE_COUNT_FOR_STAGE[currentStageNumber-1]);
    }    
    else if(currentStageNumber == 4)
    {
        // set allowed shapes
        SHAPE_DOWN_FALL_SPEED_Y = 0.7f;
    }    

}

void DisplayContainer::showCurrentStageScreen(sf::RenderWindow& displayWindow)
{
    fContainerRef.drawSingleString (displayWindow, GameFontStrings::STAGE_LABEL);
    fContainerRef.drawSingleString (displayWindow, GameFontStrings::STAGE_VALUE);

}
