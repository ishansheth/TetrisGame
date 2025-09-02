#include "DisplayContainer.h"
#include "Util.h"
#include <filesystem>
#include <iostream>
#include <sys/stat.h>
#include <thread>
#include <string.h>
#include "MetaFileHandler.h"

float SHAPE_DOWN_FALL_SPEED_Y = 0.1;

DisplayContainer::DisplayContainer(FontContainer &fCon, ShapeGenerator &shapegenerator)
    : shapeGen(shapegenerator), lastShape(nullptr), nextShape(nullptr), scoreValue(0), isGameOverState(false),
      isGamePaused(false), fContainerRef(fCon), currentStageNumber(1), bombExplosionParticles(1000, sf::Color(255, 255, 0)),
      displayEnterUsernameScreen(false),highScoreAchieved(false), insertRowsAtbottom(false),gameComplete(false),
      windowClosePressed(false),oneMinTime(sf::seconds(60))
{
    auto yVal = LAST_ROW_Y;
    for (int i = NUMBER_OF_ROWS_IN_GAME; i > 0; i--)
    {
        individualComponentContainer[yVal] = std::vector<std::pair<sf::RectangleShape **, IShape *>>();
        rowYCoordinate.push_back(yVal);
        yVal -= SQUARE_SIDE_LENGTH_WITH_OUTLINE;
    }

    if (!shapeSettleSoundBuffer.loadFromFile(std::getenv("HOME") + std::string(TOSTRINGYFY(SOUND_FOLDER_PATH)) +
                                             std::string(BLIP_SOUND_FILE_NAME)))
    {
        std::cout << "Could not load blip-131856.wav file" << std::endl;
    }

    if (!rowRemovedExplosionSoundBuffer.loadFromFile(std::getenv("HOME") + std::string(TOSTRINGYFY(SOUND_FOLDER_PATH)) +
                                                     std::string(EXPLOSION_SOUND_FILE_NAME)))
    {
        std::cout << "Could not load blip-131856.wav file" << std::endl;
    }

    if (!bombExplosionSoundBuffer.loadFromFile(std::getenv("HOME") + std::string(TOSTRINGYFY(SOUND_FOLDER_PATH)) +
                                               std::string(BOMB_EXPLOSION_SOUND_FILE_NAME)))
    {
        std::cout << "Could not load bomb_explosion.wav file" << std::endl;
    }

    shapeSettleSound.setBuffer(shapeSettleSoundBuffer);
    rowRemovedExplosionSound.setBuffer(rowRemovedExplosionSoundBuffer);
    bombExplosionSound.setBuffer(bombExplosionSoundBuffer);

    setParamtersForCurrentStage();

    for (unsigned int i = 0; i < NUMBER_OF_SQUARES_IN_ROW; i++)
    {
        rowCollapseParticleSystems.emplace_back(1000, sf::Color(255, 0, 0));
    }

    MetaFileHandler::readMetaDataFile();
    highScoreDisplayData = MetaFileHandler::getHighScoreDisplayString();
}

bool DisplayContainer::isGameOver()
{
    if (isGameOverState)
        return isGameOverState;

    if (individualComponentContainer[rowYCoordinate.back()].size() > 0)
    {
        isGameOverState = true;
    }
    return isGameOverState;
}

// check of falling shape is intersecting with currently displayed shapes
bool DisplayContainer::isIntersecting(const sf::Vector2f &shapePosition, const IShape *ignoreshape)
{
    // get all the shapes for the container which is currently displayed in window
    for (auto &s : individualComponentContainer)
    {
        // for incoming shape, iterate over all rectangles
        // check the shape in container

        for (auto &t2 : s.second)
        {
            if (t2.second == ignoreshape)
                continue;

            if (
                //                    bottom >= top
                ((shapePosition.y + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS) >= (*t2.first)->getPosition().y) &&
                //                    top <= bottom
                (shapePosition.y <= ((*t2.first)->getPosition().y + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS)) &&
                //                    left <= right
                (shapePosition.x <= ((*t2.first)->getPosition().x + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS)) &&
                //                    right >= left
                (shapePosition.x + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS) >= (*t2.first)->getPosition().x)
            {
                return true;
            }
        }
    }
    return false;
}


int DisplayContainer::getLowestYVal(const float searchX, const float refY)
{
    auto iteratorPtr = individualComponentContainer.find(refY);
    for (; iteratorPtr != individualComponentContainer.end(); iteratorPtr++)
    {
        auto searchPtr = std::find_if(iteratorPtr->second.begin(), iteratorPtr->second.end(),
                                      [&searchX](std::pair<sf::RectangleShape **, IShape *> &element) {
                                          return (std::fabs((*(element.first))->getPosition().x - searchX) <= 3.f);
                                      });
        if (searchPtr != iteratorPtr->second.end())
        {
            return std::prev(iteratorPtr)->first;
        }
    }
    return rowYCoordinate[0];
}

int DisplayContainer::getAllowedYVal(const float yCoordinate)
{
    int minIdx = -1;
    int minDiff = 1000;

    for (std::size_t i = 0; i < rowYCoordinate.size(); i++)
    {
        if (static_cast<unsigned int>(abs(rowYCoordinate[i] - yCoordinate)) < minDiff)
        {
            minDiff = static_cast<unsigned int>(abs(rowYCoordinate[i] - yCoordinate));
            minIdx = i;
        }
    }
    return rowYCoordinate[minIdx];
}

void DisplayContainer::handleKey(const sf::Keyboard::Key &key)
{
    if(isGamePaused)
    {
        return;
    }

    if (lastShape != nullptr && lastShape->isBomb() && key == sf::Keyboard::Space)
    {
        // terminate bomb before it hits bottom
        terminateBomb = true;
    }

    if (lastShape != nullptr)
    {
        lastShape->handleKey(key);
    }
}

void DisplayContainer::generateAndDrawShape(sf::RenderWindow &displayWindow)
{
    if (isGameOverState)
    {
        return;
    }

    if (lastShape == nullptr)
    {
        shapeGen.generateShapes();
        lastShape = shapeGen.getShape(sf::Vector2f(0, 0), this);
        nextShape = shapeGen.getNextShape(sf::Vector2f(NEXT_SHAPE_X, NEXT_SHAPE_Y), this);
    }

    if(!windowClosePressed)
    {
        lastShape->drawShape(displayWindow);
        nextShape->drawShape(displayWindow);
    }

    fContainerRef.setFontString(GameFontStrings::HIGH_SCORE_VALUES, highScoreDisplayData);
    fContainerRef.setFontString(GameFontStrings::SCORE_VALUE, std::to_string(scoreValue));
    fContainerRef.setFontString(GameFontStrings::STAGE_VALUE, std::to_string(currentStageNumber));

    drawDisplayContainer(displayWindow);

    if(insertRowsAtbottom && !isGamePaused)
    {
        auto timeElapsed = rowInsertionTimer.restart();
        oneMinTime -= timeElapsed;
        if(oneMinTime <= sf::Time::Zero)
        {
            insertRowAtBottom();
            oneMinTime = sf::seconds(60);
        }

    }
}

// raise the mud from bottom which means insert rows of single squares with brown color
void DisplayContainer::insertRowAtBottom()
{
    // shift whole structre up by one row
    for(auto it = individualComponentContainer.begin(); it != individualComponentContainer.end(); it++)
    {
        if(it->second.size() > 0)
        {
            auto prevRowPtr = it;
            prevRowPtr--;
            for(auto& element : it->second)
            {   
                auto xpos = (*(element.first))->getPosition().x;
                (*(element.first))->setPosition(xpos,prevRowPtr->first);
                prevRowPtr->second.push_back(element);
            }
            it->second.clear();
        }
    }   

    // create a vector of squares
    // first get the position randomly and also get the number of squares to be inserted randomly
    static std::random_device randomDev;
    static std::mt19937 randomGenerator;
    static std::uniform_int_distribution<int> uniformDistribution(1,19);
    std::vector<unsigned int> squareIndexes;

    auto numberOfSquares = uniformDistribution(randomGenerator);
    uniformDistribution.param(std::uniform_int_distribution<int>::param_type{0, 19});

    // dont insert the squares at same index otherwise they will overlap and wrong count of squares in every row
    while(squareIndexes.size() != numberOfSquares)
    {
        auto idx = uniformDistribution(randomGenerator);
        if(std::find(squareIndexes.begin(), squareIndexes.end(), idx) == squareIndexes.end())
        {
            // idx not found then add
            squareIndexes.push_back(idx);
        }
    }

    std::vector<std::pair<sf::RectangleShape **, IShape *>> newRowAtBottom;

    for(unsigned int idx : squareIndexes)
    {        
        auto* s = shapeGen.getSingleSquareShape(idx,std::prev(individualComponentContainer.end())->first);
        for(auto& element : s->getShapeContainer())
        {
            newRowAtBottom.push_back(std::make_pair(element,s));
        }
    }
    // added row at the bottom
    std::prev(individualComponentContainer.end())->second.clear();
    for(auto& element : newRowAtBottom)
    {
        std::prev(individualComponentContainer.end())->second.push_back(element);
    }
}

void DisplayContainer::terminateBombEarly(sf::RenderWindow &displayWindow)
{
    const float ypos = (*(lastShape->getShapeContainer()[0]))->getPosition().y;
    const float minx = (*(lastShape->getShapeContainer()[0]))->getPosition().x;
    const float maxx = minx + (3.f * SQUARE_SIDE_LENGTH_WITH_OUTLINE);

    bombExplosionParticles.setEmitter(sf::Vector2f((maxx + minx) * 0.5f, ypos));

    // bomb explosion sound runs about 4000 ms and while loop has delay of 100ms
    // so set the count 4000/100 so that whole sound is heard when explosion particle effect is displayed in the loop
    unsigned int count = 4000 / 100;

    bool blocksRemoved = false;

    bombExplosionSound.play();

    while (count > 0)
    {
        displayWindow.clear(sf::Color::Black);

        prepareDefaultScreenItems(displayWindow);

        bombExplosionParticles.update();

        displayWindow.draw(bombExplosionParticles);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        drawDisplayContainer(displayWindow);

        displayWindow.display();

        count--;

    }
}

void DisplayContainer::handleBombDrop(sf::RenderWindow &displayWindow)
{
    int minx = static_cast<int>((*(lastShape->getShapeContainer()[0]))->getPosition().x) - SQUARE_SIDE_LENGTH_WITH_OUTLINE;
    if(minx < 0)
    {
        minx = 0;
    }
    unsigned int maxx = minx + (3 * SQUARE_SIDE_LENGTH_WITH_OUTLINE);

    std::vector<unsigned int> removeFromRows;

    for (auto it = individualComponentContainer.begin(); it != individualComponentContainer.end(); it++)
    {
        for (auto &element : it->second)
        {
            auto x = static_cast<int>((*(element.first))->getPosition().x);
            if (x >= minx && x <= maxx)
            {
                removeFromRows.push_back(it->first);
                break;
            }
        }

        if (removeFromRows.size() == NO_ROWS_DESTROYED_BY_BOMB)
            break;
    }

    if (removeFromRows.size() == 0)
    {
        return;
    }

    bombExplosionParticles.setEmitter(sf::Vector2f((maxx + minx) / 2, removeFromRows[0]));

    // bomb explosion sound runs about 4000 ms and while loop has delay of 100ms
    // so set the count 4000/100 so that whole sound is heard when explosion particle effect is displayed in the loop
    unsigned int count = 4000 / 100;

    bool blocksRemoved = false;

    bombExplosionSound.play();

    while (count > 0)
    {
        displayWindow.clear(sf::Color::Black);

        prepareDefaultScreenItems(displayWindow);

        bombExplosionParticles.update();

        displayWindow.draw(bombExplosionParticles);

        if (!blocksRemoved)
        {
            blocksRemoved = true;
            for (auto &rowY : removeFromRows)
            {
                std::sort(
                    individualComponentContainer[rowY].begin(), individualComponentContainer[rowY].end(),
                    [](std::pair<sf::RectangleShape **, IShape *> &a, std::pair<sf::RectangleShape **, IShape *> &b) {
                        return (**(a.first)).getPosition().x < (**(b.first)).getPosition().x;
                    });

                for (auto &element : individualComponentContainer[rowY])
                {
                    auto xpos = static_cast<int>((*(element.first))->getPosition().x);
                    auto ypos = static_cast<int>((*(element.first))->getPosition().y);

                    if (xpos >= minx && xpos <= maxx)
                    {
                        (element.second)->setBroken();
                        delete *(element.first);
                        *(element.first) = nullptr;
                    }
                }

                if (individualComponentContainer[rowY].size() > 0)
                {
                    individualComponentContainer[rowY].erase(
                        std::remove_if(individualComponentContainer[rowY].begin(),
                                       individualComponentContainer[rowY].end(),
                                       [](std::pair<sf::RectangleShape **, IShape *> &element) {
                                           return (*(element.first) == nullptr);
                                       }),
                        individualComponentContainer[rowY].end());
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        drawDisplayContainer(displayWindow);

        displayWindow.display();

        count--;
    }

    const int bottomRowY = std::prev(individualComponentContainer.end())->first;
    const int nextAboveBottom = bottomRowY - SQUARE_SIDE_LENGTH_WITH_OUTLINE;

    for (auto it = individualComponentContainer.rbegin(); it != individualComponentContainer.rend(); it++)
    {
        if(it->first <= nextAboveBottom)
        {
            makeRowFall(it->first, displayWindow);
        }
    }
}

void DisplayContainer::processshapes(sf::RenderWindow &displayWindow)
{
    if (isGameOverState)
    {
        return;
    }

    if (lastShape == nullptr)
    {
        return;
    }

    if (lastShape->isBomb() && terminateBomb)
    {
        terminateBombEarly(displayWindow);
        terminateBomb = false;
        delete lastShape;
        lastShape = nullptr;
        return;
    }


    auto moveStatus = lastShape->getMoveStatus();
    if (!moveStatus)
    {

        // mapping individual component of shape to its y co-ordinate

        if (lastShape->isBomb())
        {
            handleBombDrop(displayWindow);
        }
        else
        {
            for (auto &s : lastShape->getShapeContainer())
            {
                auto yVal = getAllowedYVal((*s)->getPosition().y);
                auto xVal = (*s)->getPosition().x;
                (*s)->setPosition(sf::Vector2f(xVal,yVal));
                individualComponentContainer[yVal].push_back(std::make_pair(s, lastShape));
            }
            shapeSettleSound.play();
        }

        checkFullRows(displayWindow);
        lastShape = nullptr;
    }
}

void DisplayContainer::drawDisplayContainer(sf::RenderWindow &displayWindow)
{
    for (auto &s : individualComponentContainer)
    {
        for (auto &element : s.second)
        {
            if (*(element.first) != nullptr)
                displayWindow.draw(**(element.first));
        }
    }
}

void DisplayContainer::prepareDefaultScreenItems(sf::RenderWindow &displayWindow)
{

    displayWindow.draw(borderLine1, 2, sf::Lines);
    displayWindow.draw(borderLine2, 2, sf::Lines);
    displayWindow.draw(borderLine3, 2, sf::Lines);
    displayWindow.draw(borderLine4, 2, sf::Lines);
    displayWindow.draw(highscore_partition_line1, 2, sf::Lines);
    displayWindow.draw(highscore_partition_line2, 2, sf::Lines);

    displayWindow.draw(partitionLine, 2, sf::Lines);

    fContainerRef.drawFonts(displayWindow);
    nextShape->drawShape(displayWindow);
}

void DisplayContainer::makeRowFall(unsigned int sourceY, sf::RenderWindow &displayWindow)
{

    // to move the row down, you have to animate
    // steps to animate:
    // clear the screen
    // draw default objects
    // take the whole container and draw it
    // take the row which needs to fall and move it and then draw it
    // then display it
    const sf::Vector2f fallVelocity = {SHAPE_DOWN_FALL_SPEED_X, SHAPE_DOWN_FALL_SPEED_Y};

    bool wasShapeMoved = true;
    bool wholeShapeMoved = false;
    bool wasBrokenShapeMoved = false;

    std::set<IShape *> movedWholeShapes;
    std::set<sf::RectangleShape **> movedBrokenShapes;
    std::set<IShape *> uniqueWholeShapes;

    std::unordered_map<int, std::vector<int>> wholeShapeXY;

    for (auto &e : individualComponentContainer[sourceY])
    {
        if (!(e.second)->isShapeBroken())
        {
            if (uniqueWholeShapes.find((e.second)) == uniqueWholeShapes.end())
            {
                uniqueWholeShapes.insert((e.second));
                for (auto &s : ((e.second))->getShapeContainer())
                {
                    auto xval = (*(s))->getPosition().x;
                    auto yval = getAllowedYVal((*(s))->getPosition().y);
                    wholeShapeXY[yval].push_back(xval);
                }
            }
        }
    }
    uniqueWholeShapes.clear();

    while (wasShapeMoved)
    {
        displayWindow.clear(sf::Color::Black);

        prepareDefaultScreenItems(displayWindow);

        if (individualComponentContainer[sourceY].size() > 0)
        {
            wasShapeMoved = false;

            for (auto &e : individualComponentContainer[sourceY])
            {
                if(!(e.second)->canShapeFall())
                {
                    continue;
                }

                auto xval = (*(e.first))->getPosition().x;
                auto p1 = (*(e.first))->getPosition() + fallVelocity;
                auto lowestPossibleY = getLowestYVal(xval, sourceY+SQUARE_SIDE_LENGTH_WITH_OUTLINE);
                
                if ((e.second)->isShapeBroken() && p1.y <= lowestPossibleY)
                {
                    (*(e.first))->move(fallVelocity);
                    movedBrokenShapes.insert((e.first));
                    wasShapeMoved = true;
                    wasBrokenShapeMoved = true;
                }
                else if ((e.second)->moveShape())
                {
                    wasShapeMoved = true;
                    wholeShapeMoved = true;
                    movedWholeShapes.insert((e.second));
                }
            }
        }
        else
        {
            wasShapeMoved = false;
        }

        drawDisplayContainer(displayWindow);

        displayWindow.display();
    }

    for (auto &e : individualComponentContainer[sourceY])
    {
        auto *shapeInterface = e.second;
        auto **rectangle = e.first;
        if (shapeInterface->isShapeBroken() && movedBrokenShapes.find(rectangle) != movedBrokenShapes.end())
        {
            const auto ypos = getAllowedYVal((*(rectangle))->getPosition().y);
            const auto xpos = (*rectangle)->getPosition().x;
            (*rectangle)->setPosition(sf::Vector2f(xpos,ypos));
            individualComponentContainer[ypos].push_back(std::make_pair(rectangle, shapeInterface));
        }
        else if(!shapeInterface->isShapeBroken() && uniqueWholeShapes.find(shapeInterface) == uniqueWholeShapes.end())
        {
            // the shape was moved because found in set
            if (movedWholeShapes.find(shapeInterface) != movedWholeShapes.end())
            {
                uniqueWholeShapes.insert(shapeInterface);

                auto squares = shapeInterface->getShapeContainer();
                for (auto &s : squares)
                {
                    const auto yval = getAllowedYVal((*s)->getPosition().y);
                    const auto xval = (*s)->getPosition().x;
                    (*s)->setPosition(sf::Vector2f(xval,yval));

                    individualComponentContainer[yval].push_back(std::make_pair(s, shapeInterface));
                }
            }
        }
    }

    if (wholeShapeMoved)
    {
        for (const auto &[originalY, originalXs] : wholeShapeXY)
        {
            for (auto &x1 : originalXs)
            {
                // if whole shape like stick which is vertical falls, then new position might overlap with older
                // position so only remove single square  in case they overlap, not 2 and for that count variable is
                // used if count is more that 0 means we have seen this square at this position and dont remove it
                unsigned int count = 0;
                individualComponentContainer[originalY].erase(
                    std::remove_if(individualComponentContainer[originalY].begin(),
                                   individualComponentContainer[originalY].end(),
                                   [&count, &x1, movedWholeShapes](std::pair<sf::RectangleShape **, IShape *> &element) {
                                       if ((std::fabs((*(element.first))->getPosition().x - x1) < 2.f) &&
                                           !element.second->isShapeBroken() && 
                                           count == 0 && 
                                           movedWholeShapes.find(element.second) != movedWholeShapes.end())
                                       {
                                           count++;
                                           return true;
                                       }
                                       else
                                       {
                                           return false;
                                       }
                                   }),
                    individualComponentContainer[originalY].end());
            }
        }
    }

    if(wasBrokenShapeMoved)
    {

        if (individualComponentContainer[sourceY].size() > 0)
        {
            individualComponentContainer[sourceY].erase(
                std::remove_if(
                    individualComponentContainer[sourceY].begin(), individualComponentContainer[sourceY].end(),
                    [movedBrokenShapes](std::pair<sf::RectangleShape **, IShape *> &element) 
                    { 
                        return ((element.second)->isShapeBroken() && movedBrokenShapes.find(element.first) != movedBrokenShapes.end()); 
                    }),
                individualComponentContainer[sourceY].end());
        }
    }

}

void DisplayContainer::eraseCompletedRow(int removedRowY, sf::RenderWindow &displayWindow)
{
    // sorting here is required because we have to make row disappear from left to right
    // so sort in ascending order of x-coordinate value
    std::sort(individualComponentContainer[removedRowY].begin(), individualComponentContainer[removedRowY].end(),
              [](std::pair<sf::RectangleShape **, IShape *> &a, std::pair<sf::RectangleShape **, IShape *> &b) {
                  return (**(a.first)).getPosition().x < (**(b.first)).getPosition().x;
              });

    unsigned int idx = 0;
    for (auto &element : individualComponentContainer[removedRowY])
    {
        rowCollapseParticleSystems[idx].setEmitter((*(element.first))->getPosition());
        idx += 1;
    }

    idx = 0;

    rowRemovedExplosionSound.play();

    for (auto &element : individualComponentContainer[removedRowY])
    {
        displayWindow.clear(sf::Color::Black);

        prepareDefaultScreenItems(displayWindow);

        for (unsigned int i = 0; i <= idx; i++)
        {
            rowCollapseParticleSystems[i].update();
            displayWindow.draw(rowCollapseParticleSystems[i]);
        }
        idx += 1;

        delete *element.first;
        *element.first = nullptr;
        element.second->setBroken();

        drawDisplayContainer(displayWindow);

        displayWindow.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    individualComponentContainer[removedRowY].clear();
}

void DisplayContainer::shiftStructureDownward(sf::RenderWindow &displayWindow, unsigned int yVal)
{
    eraseCompletedRow(yVal, displayWindow);
    scoreValue += SCORE_PER_ROW;

    const int bottomRowY = std::prev(individualComponentContainer.end())->first;
    const int nextAboveBottom = bottomRowY - SQUARE_SIDE_LENGTH_WITH_OUTLINE;

    for (auto it = individualComponentContainer.rbegin(); it != individualComponentContainer.rend(); it++)
    {
        if(it->first <= nextAboveBottom)
        {
            makeRowFall(it->first,displayWindow);
        }
    }
}

void DisplayContainer::checkFullRows(sf::RenderWindow &displayWindow)
{
    while (1)
    {
        bool removeRows = false;
        for (auto it = individualComponentContainer.rbegin(); it != individualComponentContainer.rend(); it++)
        {
            std::cout << it->first << "----" << it->second.size() << std::endl;
            if (it->second.size() == NUMBER_OF_SQUARES_IN_ROW)
            {
                shiftStructureDownward(displayWindow, it->first);
                removeRows = true;
            }
        }

        if (!removeRows)
        {
            break;
        }
    }
}

void DisplayContainer::handleGameState(sf::RenderWindow &displayWindow)
{
    if(gameComplete)
    {
        showGameCompleteScreenAndExit(displayWindow); 
    }
    else if (isGameOver() || windowClosePressed)
    {
        if (scoreValue > 0 && scoreValue > MetaFileHandler::getMinHighScore())
        {
            if (highScoreUsername.size() == 0)
            {
                displayEnterUsernameScreen = true;
            }

            currentscore = scoreValue;
            highScoreAchieved = true;
        }
        cleanDisplayContainer();
        scoreValue = 0;
        if(!windowClosePressed)
        {
            fContainerRef.drawSingleString(displayWindow, GameFontStrings::GAME_OVER);
        }

        if(windowClosePressed && !displayEnterUsernameScreen && highScoreAchieved)
        {
            // window closed with new high score and known username, just show a message string and exit
            displayWindow.clear();
            displayWindow.draw(borderLine1, 2, sf::Lines);
            displayWindow.draw(borderLine2, 2, sf::Lines);
            displayWindow.draw(borderLine3, 2, sf::Lines);
            displayWindow.draw(borderLine4, 2, sf::Lines);
            fContainerRef.drawSingleString(displayWindow, GameFontStrings::WINDOW_CLOSE_HIGHSCORE_SAVED);
            displayWindow.display();
            std::this_thread::sleep_for (std::chrono::milliseconds (1000));
        }

        if (displayEnterUsernameScreen)
        {
            bool shiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);

            for(unsigned int key = sf::Keyboard::A; key <= sf::Keyboard::Z; key++)
            {
                if(sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key)))
                {
                    std::this_thread::sleep_for (std::chrono::milliseconds (90));
                    if(sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key)))
                    {
                        char letter = 'A' + (key - sf::Keyboard::A);
                        highScoreUsername += shiftPressed ? letter : std::tolower(letter);    
                        break;
                    }                        
                }
            }

            for(unsigned int key = sf::Keyboard::Num0; key <= sf::Keyboard::Num9; key++)
            {
                if(sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key)))
                {
                    std::this_thread::sleep_for (std::chrono::milliseconds (90));
                    if(sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key)))
                    {
                        highScoreUsername += std::to_string(key - sf::Keyboard::Num0);    
                        break;
                    }                        
                }
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
            {
                std::this_thread::sleep_for (std::chrono::milliseconds (90));
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
                {
                    if(highScoreUsername.size() > 0)
                    {
                        highScoreUsername.pop_back();   
                    }

                }

            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                std::this_thread::sleep_for (std::chrono::milliseconds (90));
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                {
                    displayEnterUsernameScreen = false;
                    highScoreAchieved = false;
                    MetaFileHandler::updateNewHighScore(currentscore, highScoreUsername);
                }
            }

        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F10))
        {
            isGameOverState = false;
            highScoreDisplayData.clear();
            cleanDisplayContainer();
            MetaFileHandler::updateNewHighScore(currentscore, highScoreUsername);
            highScoreDisplayData = MetaFileHandler::getHighScoreDisplayString();
            generateAndDrawShape(displayWindow);
            highScoreAchieved = false;

        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            cleanDisplayContainer();
            MetaFileHandler::saveMetaDataFileAndClose();
            displayWindow.close();
        }

        // Complex if else ladder based on different scenario
        if (displayEnterUsernameScreen)
        {
            fContainerRef.setFontString(GameFontStrings::USERNAME_INPUT_STRING, highScoreUsername);
            fContainerRef.drawSingleString(displayWindow, GameFontStrings::HIGHSCORE_ACHIEVED);
            fContainerRef.drawSingleString(displayWindow, GameFontStrings::ENTER_USERNAME);
            fContainerRef.drawSingleString(displayWindow, GameFontStrings::USERNAME_INPUT_STRING);
        }
        else
        {
            // show the string only if high score was achieved
            if (highScoreAchieved)
            {
                fContainerRef.drawSingleString(displayWindow, GameFontStrings::HIGHSCORE_ACHIEVED);
            }

            // if the window close was not done and show user instruction
            if(!windowClosePressed)
            {
                fContainerRef.drawSingleString(displayWindow, GameFontStrings::GAME_OVER_USER_SELECTION);
            }
            else
            {
                // if window was closed: then update the high score in file if the username was not taken from user
                // if the user entered the username then high score was updated already above
                if(!displayEnterUsernameScreen)
                {
                    MetaFileHandler::updateNewHighScore(currentscore, highScoreUsername);
                }
                MetaFileHandler::saveMetaDataFileAndClose();
                displayWindow.close();
            }
        }
    }
    else if (isGamePaused)
    {
        fContainerRef.drawSingleString(displayWindow, GameFontStrings::GAME_PAUSED);
        fContainerRef.drawFonts(displayWindow);
        displayWindow.draw(partitionLine, 2, sf::Lines);
        displayWindow.draw(highscore_partition_line1, 2, sf::Lines);
        displayWindow.draw(highscore_partition_line2, 2, sf::Lines);
    }
    else if (scoreValue >= CLEARING_SCORE_PER_STAGE[currentStageNumber - 1])
    {
        // steps for change stage:
        // show complete stage message
        // wait 1 sec
        // show new stage number message
        // clear the container
        // set the new parameters for the new stage, generate new shapes
        // display
        std::cout << "============stage complete" << std::endl;
        // clear the container
        cleanDisplayContainer();

        // clear the screen
        displayWindow.clear(sf::Color::Black);

        // show stage complete message
        std::string stageCompleteMsg = "Stage " + std::to_string(currentStageNumber) + " Complete!!";
        fContainerRef.setFontString(GameFontStrings::STAGE_COMPLETE_MESSAGE, stageCompleteMsg);
        fContainerRef.drawSingleString(displayWindow, GameFontStrings::STAGE_COMPLETE_MESSAGE);

        // draw border line
        displayWindow.draw(borderLine1, 2, sf::Lines);
        displayWindow.draw(borderLine2, 2, sf::Lines);
        displayWindow.draw(borderLine3, 2, sf::Lines);
        displayWindow.draw(borderLine4, 2, sf::Lines);

        // display the message
        displayWindow.display();
        // wait 1 sec
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        // clear the screen
        displayWindow.clear(sf::Color::Black);

        currentStageNumber++;
        showCurrentStageScreen(displayWindow);
        // set the parameter for the next stage

        lastShape = nullptr;
        setParamtersForCurrentStage();
        // display the necessary fonts
        fContainerRef.drawFonts(displayWindow);
        // display partition line
        displayWindow.draw(partitionLine, 2, sf::Lines);
    }
    else if (!isGamePaused && !isGameOverState)
    {
        moveShapes();
        fContainerRef.drawFonts(displayWindow);
        displayWindow.draw(partitionLine, 2, sf::Lines);
        displayWindow.draw(highscore_partition_line1, 2, sf::Lines);
        displayWindow.draw(highscore_partition_line2, 2, sf::Lines);
    }
    
    displayWindow.draw(borderLine1, 2, sf::Lines);
    displayWindow.draw(borderLine2, 2, sf::Lines);
    displayWindow.draw(borderLine3, 2, sf::Lines);
    displayWindow.draw(borderLine4, 2, sf::Lines);

    displayWindow.display();
    displayWindow.clear(sf::Color::Black);
}

void DisplayContainer::showGameCompleteScreenAndExit(sf::RenderWindow &displayWindow)
{
    displayWindow.clear(sf::Color::Black);
    // draw border line
    displayWindow.draw(borderLine1, 2, sf::Lines);
    displayWindow.draw(borderLine2, 2, sf::Lines);
    displayWindow.draw(borderLine3, 2, sf::Lines);
    displayWindow.draw(borderLine4, 2, sf::Lines);
    fContainerRef.drawSingleString(displayWindow, GameFontStrings::GAME_COMPLETE_MESSAGE);
    displayWindow.display();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    displayWindow.clear(sf::Color::Black);
    MetaFileHandler::saveMetaDataFileAndClose();
    cleanDisplayContainer();

    displayWindow.close ();   
    exit(1);
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
    SHAPE_DOWN_FALL_SPEED_Y = FALL_SPEED_FOR_STAGE[currentStageNumber - 1];
    insertRowsAtbottom = true;

    if (currentStageNumber == 1)
    {
        shapeGen.setAllowedShapesCount(SHAPE_COUNT_FOR_STAGE[currentStageNumber - 1]);
    }
    else if (currentStageNumber == 2)
    {
        shapeGen.setAllowedShapesCount(SHAPE_COUNT_FOR_STAGE[currentStageNumber - 1]);
    }
    else if (currentStageNumber == 3)
    {
        shapeGen.setAllowedShapesCount(SHAPE_COUNT_FOR_STAGE[currentStageNumber - 1]);
    }
    else if (currentStageNumber == 4)
    {
        // TODO: in the last stage, the whole structre will move upwards at regular interval, maybe 1 min
        // for this add a row at the bottom with random x values
        // or offer one more shape or power to the user 
        insertRowsAtbottom = true;
    }
    else
    {
        // game complete message and show score, then close
        gameComplete = true;
    }
}


void DisplayContainer::showCurrentStageScreen(sf::RenderWindow &displayWindow)
{
    // clear the screen
    displayWindow.clear(sf::Color::Black);

    // draw border line
    displayWindow.draw(borderLine1, 2, sf::Lines);
    displayWindow.draw(borderLine2, 2, sf::Lines);
    displayWindow.draw(borderLine3, 2, sf::Lines);
    displayWindow.draw(borderLine4, 2, sf::Lines);

    // show next stage message
    auto stageCompleteMsg = std::to_string(currentStageNumber);
    // display next stage number message
    fContainerRef.setFontString(GameFontStrings::STAGE_VALUE, stageCompleteMsg);
    fContainerRef.drawSingleString(displayWindow, GameFontStrings::STAGE_LABEL, STAGE_COMPLETE_MSG_X,
                                   STAGE_COMPLETE_MSG_Y);
    fContainerRef.drawSingleString(displayWindow, GameFontStrings::STAGE_VALUE, STAGE_COMPLETE_MSG_X + 80,
                                   STAGE_COMPLETE_MSG_Y);
    // display
    displayWindow.display();
    // wait
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // clear
    displayWindow.clear(sf::Color::Black);

    // reset the position of stage label and values string
    fContainerRef.drawSingleString(displayWindow, GameFontStrings::STAGE_LABEL,
                                   stringToLocation.at(GameFontStrings::STAGE_LABEL).x,
                                   stringToLocation.at(GameFontStrings::STAGE_LABEL).y);
    fContainerRef.drawSingleString(displayWindow, GameFontStrings::STAGE_VALUE,
                                   stringToLocation.at(GameFontStrings::STAGE_VALUE).x,
                                   stringToLocation.at(GameFontStrings::STAGE_VALUE).y);
}

void DisplayContainer::cleanDisplayContainer()
{
    for (auto it = individualComponentContainer.begin(); it != individualComponentContainer.end(); it++)
    {
        for (auto &element : it->second)
        {
            if (element.first != nullptr)
            {
                delete *element.first;
            }
            *element.first = nullptr;
        }
        it->second.clear();
    }
}

void DisplayContainer::handleWindowCloseEvent()
{
    windowClosePressed = true;    
}