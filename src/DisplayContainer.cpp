#include "DisplayContainer.h"
#include <iostream>
#include <thread>
#include "Util.h"

DisplayContainer::DisplayContainer (FontContainer& fCon, ShapeGenerator& shapegenerator)
: shapeGen(shapegenerator), 
    lastShape (nullptr), 
    nextShape (nullptr), 
    moveStatus (true),
    scoreValue (0), 
    isGameOverState (false),
    isGamePaused(false), 
    fContainerRef (fCon), 
    currentStageNumber(1),
    xCoordinateEmitter(-100)
    {

    auto yVal = LAST_ROW_Y;
    for (int i = NUMBER_OF_ROWS_IN_GAME; i > 0; i--) {
        individualComponentContainer[yVal] =
        std::vector<std::pair<sf::RectangleShape**, IShape*>> ();
        rowYCoordinate.push_back(yVal);
        yVal -= SQUARE_SIDE_LENGTH_WITH_OUTLINE;
    }

    if(!shapeSettleSoundBuffer.loadFromFile(std::getenv ("HOME") + 
    std::string (TOSTRINGYFY (SOUND_FOLDER_PATH)) + 
    std::string(BLIP_SOUND_FILE_NAME)))
    {
        std::cout<<"Could not load blip-131856.wav file"<<std::endl;
    }

    if(!rowRemovedExplosionSoundBuffer.loadFromFile(std::getenv ("HOME") + 
    std::string (TOSTRINGYFY (SOUND_FOLDER_PATH)) + 
    std::string(EXPLOSION_SOUND_FILE_NAME)))
    {
        std::cout<<"Could not load blip-131856.wav file"<<std::endl;
    }


    shapeSettleSound.setBuffer(shapeSettleSoundBuffer);
    rowRemovedExplosionSound.setBuffer(rowRemovedExplosionSoundBuffer);
    setParamtersForCurrentStage();
    
    
    for(unsigned int i = 0; i < NUMBER_OF_SQUARES_IN_ROW; i++)
    {
        rowCollapseParticleSystems.emplace_back(1000);
    }

}


bool DisplayContainer::isGameOver () {
    return (individualComponentContainer[FIRST_ROW_Y].size () > 0);
}

// check of falling shape is intersecting with currently displayed shapes
bool DisplayContainer::isIntersecting (sf::Vector2f shapePosition) {
    // get all the shapes for the container which is currently displayed in window
    for (auto& s : individualComponentContainer) {
        // for incoming shape, iterate over all rectangles
        // check the shape in container
        
        for (auto& t2 : s.second) 
        {
            if (
            //                    bottom >= top
            shapePosition.y + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS >=
            (*t2.first)->getPosition ().y - SQUARE_OUTLINE_THICKNESS &&
            //                    top <= bottom
            shapePosition.y <= (*t2.first)->getPosition ().y + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS &&
            //                    left <= right
            shapePosition.x <= (*t2.first)->getPosition ().x + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS &&
            //                    right >= left
            shapePosition.x + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS >=
            (*t2.first)->getPosition ().x)

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
        if (it->first > refY) 
        {
            bool found = false;
            for (auto& element : it->second) 
            {
                if (abs((*element.first)->getPosition ().x - x) <= 3) 
                {
                    it--;
                    return it->first;
                }
            }
        }
    }
    return rowYCoordinate.front();
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


    lastShape->drawShape (displayWindow);
    nextShape->drawShape (displayWindow);

    for (auto& s : individualComponentContainer) {
        for (auto& element : s.second) {
            displayWindow.draw (**(element.first));
        }
    }


    std::string displayScoreVal = std::to_string (getScore ());
    fContainerRef.setFontString (GameFontStrings::SCORE_VALUE, displayScoreVal);

    std::string stageVal = std::to_string (currentStageNumber);
    fContainerRef.setFontString (GameFontStrings::STAGE_VALUE, stageVal);
}


void DisplayContainer::handleBombDrop(IShape* bombShape)
{
    float minx = 1000;
    float maxx = 0;

    for (auto& s : lastShape->getShapeContianer ()) 
    {
        minx = std::min(minx,(*s)->getPosition ().x);
        maxx = std::max(maxx,(*s)->getPosition ().x);        
    }

    int countRows = 3;
    for (auto& s : individualComponentContainer) 
    {
        bool blocksRemoved = false;
        for (auto& element : s.second) 
        {
            auto x = static_cast<int>((*(element.first))->getPosition().x);
            if(x >= minx && x <= maxx)
            {

                delete *element.first;
                *element.first = nullptr;
                element.second->setBroken();   
                blocksRemoved = true;             
            }
        }

        if(blocksRemoved)
        {
            countRows--;
        }

        if(countRows == 0)
            break;
            
    }    

}


void DisplayContainer::processshapes (sf::RenderWindow& displayWindow) 
{
    if (isGameOverState) {
        return;
    }

    if(lastShape == nullptr)
    {
        return;
    }

    moveStatus = lastShape->getMoveStatus();
    if (!moveStatus) 
    {

        // mapping individual component of shape to its y co-ordinate
        for (auto& s : lastShape->getShapeContianer ()) 
        {
            auto yVal = getAllowedYVal((*s)->getPosition ().y);
            individualComponentContainer[yVal].push_back (std::make_pair (s, lastShape));
        }

        if(lastShape->isBomb())
        {
            handleBombDrop(lastShape);
        }

        checkFullRows (displayWindow);
        lastShape = nullptr;
        shapeSettleSound.play();

    }
}

void DisplayContainer::prepareDefaultScreenItems(sf::RenderWindow& displayWindow)
{

    displayWindow.draw (borderLine1, 2, sf::Lines);
    displayWindow.draw (borderLine2, 2, sf::Lines);
    displayWindow.draw (borderLine3, 2, sf::Lines);
    displayWindow.draw (borderLine4, 2, sf::Lines);

    displayWindow.draw (partitionLine, 2, sf::Lines);


    fContainerRef.drawFonts (displayWindow);
    nextShape->drawShape (displayWindow);

}

void DisplayContainer::makeRowFall(int sourceY, int removedRow, sf::RenderWindow& displayWindow)
{

    // to move the row down, you have to animate
    // steps to animate:
    // clear the screen
    // draw default objects
    // take the whole container and draw it
    // take the row which needs to fall and move it and then draw it
    // then display it
    sf::Vector2f fallVelocity = {0,0.2f};

    /*

        how would you move the pieces? some are broken and some are full shapes

        if you use IShape interface to move the pieces, then it will moove the shape from different 
        rows and you cant control

        so you have to take induiidual shquares and drop them. but you have to see until which point
        then should be dropped because for whole shape its different and for broken shape its different

    */

    unsigned int rowCnt = 0;
    auto iterator = individualComponentContainer.find(sourceY);
    for(;iterator != individualComponentContainer.begin();iterator--)
    {
        if(iterator->second.size() > 0)
        {
            rowCnt++;
        }
    }



    while(rowCnt > 0)
    {
        auto iterator = individualComponentContainer.find(sourceY);
        displayWindow.clear(sf::Color::Black);
        prepareDefaultScreenItems(displayWindow);
        auto removedRow_r = removedRow;

        for(;iterator != individualComponentContainer.begin();iterator--)
        {
            bool wasShapeMoved = false;
            
            if(iterator->second.size() > 0)
            {
                for(auto& element: iterator->second)    
                {
                    auto xval = (*(element.first))->getPosition().x;
                    auto p1 = (*(element.first))->getPosition() + fallVelocity; 

                    if((element.second)->isShapeBroken() && p1.y <= getLowestYVal(xval,removedRow_r))
                    {
                        (*(element.first))->move(fallVelocity);
                        wasShapeMoved = true;                    

                    }
                    
                    else if(!(element.second)->isShapeBroken() && p1.y <= removedRow_r)   
                    {
                        (*(element.first))->move(fallVelocity);
                        wasShapeMoved = true;                    
                    }
                }
                
                // if wasShapeMoved was not set to true then nothing moved, 
                // so add that row in container and remove from source 
                if(!wasShapeMoved)
                {
                    rowCnt--;
                    for(auto& e: iterator->second)
                    {
                        if((e.second)->isShapeBroken())
                        {
                            auto ypos = getAllowedYVal((*(e.first))->getPosition ().y);
                            individualComponentContainer[ypos].push_back(e);
                        }
                        else
                        {
                            individualComponentContainer[removedRow_r].push_back(e);
                        }

                    }

                    iterator->second.clear();
                    removedRow_r -= SQUARE_SIDE_LENGTH_WITH_OUTLINE;

                }

            }
            else
            {
                wasShapeMoved = true;
            }
            

            for (auto& s : individualComponentContainer) {
                for (auto& element : s.second) {
                    displayWindow.draw (**(element.first));
                }
            }    

        }


        displayWindow.display();

    }

}


void DisplayContainer::eraseCompletedRow(int removedRowY, sf::RenderWindow& displayWindow)
{
    // sorting here is required because we have to make row disappear from left to right
    // so sort in ascending order of x-coordinate value
    std::sort(individualComponentContainer[removedRowY].begin(), 
                individualComponentContainer[removedRowY].end(), 
                [](std::pair<sf::RectangleShape**, IShape*>& a, std::pair<sf::RectangleShape**, IShape*>& b)
                {
                    return (**(a.first)).getPosition().x < (**(b.first)).getPosition().x; 
                } );

    unsigned int interval = 0;

    unsigned int idx = 0;
    for(auto& element: individualComponentContainer[removedRowY])
    {
       rowCollapseParticleSystems[idx].setEmitter( (*(element.first))->getPosition());
       idx += 1;
    }

    idx = 0;

    rowRemovedExplosionSound.play();
    
    for(auto& element: individualComponentContainer[removedRowY])
    {
        displayWindow.clear(sf::Color::Black);

        prepareDefaultScreenItems(displayWindow);

        for(unsigned int i = 0; i <= idx; i++)
        {
            rowCollapseParticleSystems[i].update();
            displayWindow.draw(rowCollapseParticleSystems[i]);
        }
        idx += 1;

        delete *element.first;
        *element.first = nullptr;
        element.second->setBroken();

        for (auto& s : individualComponentContainer) {
            for (auto& element : s.second) {
                if(*(element.first) != nullptr)
                {
                    displayWindow.draw (**(element.first));
                }
            }
        }    

        displayWindow.display();
        std::this_thread::sleep_for (std::chrono::milliseconds (100));
        interval++;

    }
    individualComponentContainer[removedRowY].clear();

}

void DisplayContainer::shiftStructureDownward (sf::RenderWindow& displayWindow) {
    int startShiftYVal = 0;
    int fullRowYVal    = 0;
    bool shiftRequired = false;
    for (auto it = individualComponentContainer.rbegin ();
         it != individualComponentContainer.rend (); it++) 
    {
        if (it->second.size () == NUMBER_OF_SQUARES_IN_ROW) 
        {
            shiftRequired = true;
            if (fullRowYVal == 0) {
                fullRowYVal = it->first;
            }

            eraseCompletedRow(it->first,displayWindow);

            scoreValue += SCORE_PER_ROW;
            it++;
            startShiftYVal = it->first;
            break;
        }
    }

    if (shiftRequired) 
    {
        makeRowFall(startShiftYVal, fullRowYVal, displayWindow);
    }
    
}

void DisplayContainer::checkFullRows (sf::RenderWindow& displayWindow) {
    while (1) {
        bool removeRows = false;
        for (auto it = individualComponentContainer.rbegin ();
             it != individualComponentContainer.rend (); it++) {
            if (it->second.size () == NUMBER_OF_SQUARES_IN_ROW) {
                shiftStructureDownward (displayWindow);
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
                    delete *element.first;
                    *element.first = nullptr;

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
    else if(getScore() >= CLEARING_SCORE_PER_STAGE[currentStageNumber-1])
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
                    delete *element.first;
                    *element.first = nullptr;

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

        currentStageNumber++;
        showCurrentStageScreen(displayWindow);
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
        SHAPE_DOWN_FALL_SPEED_Y = 0.3f;
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
    // clear the screen
    displayWindow.clear (sf::Color::Black);

    // draw border line
    displayWindow.draw (borderLine1, 2, sf::Lines);
    displayWindow.draw (borderLine2, 2, sf::Lines);
    displayWindow.draw (borderLine3, 2, sf::Lines);
    displayWindow.draw (borderLine4, 2, sf::Lines);

    // show next stage message
    auto stageCompleteMsg = std::to_string(currentStageNumber);
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

}

void DisplayContainer::cleanDisplayContainer()
{
        for (auto it = individualComponentContainer.begin (); 
            it != individualComponentContainer.end (); it++) 
        {
            for(auto& element: it->second)
            {
                delete element.first;

                if(element.second != nullptr)
                {
                    delete element.second;
                    element.second = nullptr;
                }
            }            
        }

}
