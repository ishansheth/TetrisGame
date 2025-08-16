#include "DisplayContainer.h"
#include "Util.h"
#include <filesystem>
#include <iostream>
#include <sys/stat.h>
#include <thread>
#include <string.h>

float SHAPE_DOWN_FALL_SPEED_Y = 0.1;

DisplayContainer::DisplayContainer(FontContainer &fCon, ShapeGenerator &shapegenerator)
    : shapeGen(shapegenerator), lastShape(nullptr), nextShape(nullptr), scoreValue(0), isGameOverState(false),
      isGamePaused(false), fContainerRef(fCon), currentStageNumber(1), bombExplosionParticles(1000),
      displayEnterUsernameScreen(false), minHighScore(std::numeric_limits<unsigned int>::max()),
      maxHighScore(std::numeric_limits<unsigned int>::min()), highScoreAchieved(false)
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
        rowCollapseParticleSystems.emplace_back(1000);
    }

    prepeareMeatadataFile();
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
                shapePosition.y + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS >=
                    (*t2.first)->getPosition().y - SQUARE_OUTLINE_THICKNESS &&
                //                    top <= bottom
                shapePosition.y <= (*t2.first)->getPosition().y + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS &&
                //                    left <= right
                shapePosition.x <= (*t2.first)->getPosition().x + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS &&
                //                    right >= left
                shapePosition.x + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS >= (*t2.first)->getPosition().x)

            {
                return true;
            }
        }
    }
    return false;
}

int DisplayContainer::getLowestYVal(const int searchX, const int refY)
{
    auto iteratorPtr = individualComponentContainer.find(refY);
    for (; iteratorPtr != individualComponentContainer.end(); iteratorPtr++)
    {
        auto searchPtr = std::find_if(iteratorPtr->second.begin(), iteratorPtr->second.end(),
                                      [&searchX](std::pair<sf::RectangleShape **, IShape *> &element) {
                                          return (abs((*(element.first))->getPosition().x - searchX) < 2);
                                      });
        if (searchPtr != iteratorPtr->second.end())
        {
            iteratorPtr--;
            return iteratorPtr->first;
        }
    }
    return rowYCoordinate.front();
}

int DisplayContainer::getAllowedYVal(const float yCoordinate)
{
    int minIdx = -1;
    int minDiff = 1000;

    for (std::size_t i = 0; i < rowYCoordinate.size(); i++)
    {
        if (abs(rowYCoordinate[i] - yCoordinate) < minDiff)
        {
            minDiff = abs(rowYCoordinate[i] - yCoordinate);
            minIdx = i;
        }
    }
    return rowYCoordinate[minIdx];
}

void DisplayContainer::handleKey(const sf::Keyboard::Key &k)
{
    if (lastShape)
        lastShape->handleKey(k);
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

    lastShape->drawShape(displayWindow);
    nextShape->drawShape(displayWindow);

    drawDisplayContainer(displayWindow);

    fContainerRef.setFontString(GameFontStrings::HIGH_SCORE_VALUES, highScoreDisplayData);
    fContainerRef.setFontString(GameFontStrings::SCORE_VALUE, std::to_string(scoreValue));
    fContainerRef.setFontString(GameFontStrings::STAGE_VALUE, std::to_string(currentStageNumber));
}

void DisplayContainer::handleBombDrop(sf::RenderWindow &displayWindow)
{
    float minx = static_cast<int>((*(lastShape->getShapeContianer()[0]))->getPosition().x);
    float maxx = minx + (3 * SQUARE_SIDE_LENGTH_WITH_OUTLINE);

    int countRows = 3;
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

        if (removeFromRows.size() == NO_ROWS_DESROYED_BY_BOMB)
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
                    auto x = static_cast<int>((*(element.first))->getPosition().x);
                    if (x >= minx && x <= maxx)
                    {
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
            for (auto &s : lastShape->getShapeContianer())
            {
                auto yVal = getAllowedYVal((*s)->getPosition().y);
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

void DisplayContainer::makeRowFall(int sourceY, int removedRow, sf::RenderWindow &displayWindow)
{

    // to move the row down, you have to animate
    // steps to animate:
    // clear the screen
    // draw default objects
    // take the whole container and draw it
    // take the row which needs to fall and move it and then draw it
    // then display it
    const sf::Vector2f fallVelocity = {0, 0.2f};

    /*

        how would you move the pieces? some are broken and some are full shapes

        if you use IShape interface to move the pieces, then it will moove the shape from different
        rows and you cant control

        so you have to take induiidual shquares and drop them. but you have to see until which point
        then should be dropped because for whole shape its different and for broken shape its different

    */

    bool wasShapeMoved = true;
    bool wholeShapeMoved = false;

    std::set<IShape *> addedWholeShapes;

    std::unordered_map<int, std::vector<int>> wholeShapeXY;

    for (auto &e : individualComponentContainer[sourceY])
    {
        if (!(e.second)->isShapeBroken())
        {
            if (addedWholeShapes.find((e.second)) == addedWholeShapes.end())
            {
                addedWholeShapes.insert((e.second));
                for (auto &s : ((e.second))->getShapeContianer())
                {
                    auto xval = (*(s))->getPosition().x;
                    auto yval = getAllowedYVal((*(s))->getPosition().y);
                    wholeShapeXY[yval].push_back(xval);
                }
            }
        }
    }
    addedWholeShapes.clear();

    while (wasShapeMoved)
    {
        displayWindow.clear(sf::Color::Black);

        prepareDefaultScreenItems(displayWindow);

        if (individualComponentContainer[sourceY].size() > 0)
        {
            wasShapeMoved = false;

            for (auto &e : individualComponentContainer[sourceY])
            {
                auto xval = (*(e.first))->getPosition().x;
                auto p1 = (*(e.first))->getPosition() + fallVelocity;

                if ((e.second)->isShapeBroken() && p1.y <= getLowestYVal(xval, removedRow))
                {
                    (*(e.first))->move(fallVelocity);
                    wasShapeMoved = true;
                }
                else if ((e.second)->moveShape())
                {
                    wasShapeMoved = true;
                    wholeShapeMoved = true;
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
        auto *shapeInterface = (e.second);
        if (shapeInterface->isShapeBroken())
        {
            const auto ypos = getAllowedYVal((*(e.first))->getPosition().y);
            individualComponentContainer[ypos].push_back(std::make_pair(e.first, e.second));
        }
        else
        {
            if (wholeShapeMoved)
            {
                if (addedWholeShapes.find(shapeInterface) == addedWholeShapes.end())
                {
                    addedWholeShapes.insert(shapeInterface);

                    auto squares = shapeInterface->getShapeContianer();
                    for (auto &s : squares)
                    {
                        const auto yval = getAllowedYVal((*s)->getPosition().y);
                        individualComponentContainer[yval].push_back(std::make_pair(s, shapeInterface));
                    }
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
                                   [&count, &x1](std::pair<sf::RectangleShape **, IShape *> &element) {
                                       if ((static_cast<int>(abs((*(element.first))->getPosition().x - x1)) < 2) &&
                                           !element.second->isShapeBroken() && count == 0)
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

    if (individualComponentContainer[sourceY].size() > 0)
    {
        individualComponentContainer[sourceY].erase(
            std::remove_if(
                individualComponentContainer[sourceY].begin(), individualComponentContainer[sourceY].end(),
                [](std::pair<sf::RectangleShape **, IShape *> &element) { return ((element.second)->isShapeBroken()); }

                ),
            individualComponentContainer[sourceY].end());
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
    int startShiftYVal = yVal - SQUARE_SIDE_LENGTH_WITH_OUTLINE;
    int fullRowYVal = yVal;

    eraseCompletedRow(yVal, displayWindow);
    scoreValue += SCORE_PER_ROW;

    for (auto it = individualComponentContainer.rbegin(); it != individualComponentContainer.rend(); it++)
    {

        if (it->first <= startShiftYVal)
        {
            makeRowFall(startShiftYVal, fullRowYVal, displayWindow);
            startShiftYVal -= SQUARE_SIDE_LENGTH_WITH_OUTLINE;
            fullRowYVal -= SQUARE_SIDE_LENGTH_WITH_OUTLINE;
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
    if (isGameOver())
    {
        if (minHighScore == std::numeric_limits<unsigned int>::max() &&
            maxHighScore == std::numeric_limits<unsigned int>::min() && scoreValue > 0)
        {
            if (highScoreUsername.size() == 0)
            {
                displayEnterUsernameScreen = true;
            }
            minHighScore = scoreValue;
            currentscore = scoreValue;
            highScoreAchieved = true;
            cleanDisplayContainer();
        }
        else if (scoreValue > minHighScore)
        {
            if (highScoreUsername.size() == 0)
            {
                displayEnterUsernameScreen = true;
            }

            currentscore = scoreValue;
            highScoreAchieved = true;
            cleanDisplayContainer();
        }

        scoreValue = 0;
        fContainerRef.drawSingleString(displayWindow, GameFontStrings::GAME_OVER);

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

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
            {
                std::this_thread::sleep_for (std::chrono::milliseconds (90));
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
                    highScoreUsername += "0";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            {
                std::this_thread::sleep_for (std::chrono::milliseconds (90));
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
                    highScoreUsername += "1";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            {
                std::this_thread::sleep_for (std::chrono::milliseconds (90));
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
                    highScoreUsername += "2";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
            {
                std::this_thread::sleep_for (std::chrono::milliseconds (90));
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
                    highScoreUsername += "3";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
            {
                std::this_thread::sleep_for (std::chrono::milliseconds (90));
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
                    highScoreUsername += "4";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
            {
                std::this_thread::sleep_for (std::chrono::milliseconds (90));
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
                    highScoreUsername += "5";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
            {
                std::this_thread::sleep_for (std::chrono::milliseconds (90));
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
                    highScoreUsername += "6";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
            {
                std::this_thread::sleep_for (std::chrono::milliseconds (90));
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
                    highScoreUsername += "7";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
            {
                std::this_thread::sleep_for (std::chrono::milliseconds (90));
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
                    highScoreUsername += "8";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
            {
                std::this_thread::sleep_for (std::chrono::milliseconds (90));
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
                    highScoreUsername += "9";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                displayEnterUsernameScreen = false;
                highScoreAchieved = false;
                savedHighScoreData.push_back(std::make_pair(currentscore, highScoreUsername));

                if (savedHighScoreData.size() > NO_SAVED_HIGHSCORE_USERNAMES)
                {
                    std::sort(savedHighScoreData.begin(), savedHighScoreData.end(),
                                [](const auto &a, const auto &b) { return a.first > b.first; });

                    savedHighScoreData.erase(std::prev(savedHighScoreData.end()));
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F10))
        {
            isGameOverState = false;
            highScoreDisplayData.clear();
            cleanDisplayContainer();
            if (savedHighScoreData.size() > 0)
            {

                if (highScoreUsername.size() > 0)
                {
                    for (auto it = savedHighScoreData.begin(); it != savedHighScoreData.end(); it++)
                    {
                        if (it->second == highScoreUsername && currentscore > it->first)
                        {
                            it->first = currentscore;
                            break;
                        }
                    }
                }

                prepareHighscoreDisplaydata();
            }
            generateAndDrawShape(displayWindow);
            highScoreAchieved = false;


        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            cleanDisplayContainer();
            saveHighScoreInFile();
            displayWindow.close();
            metadataFileHandle.close();
        }

        if (displayEnterUsernameScreen)
        {
            fContainerRef.setFontString(GameFontStrings::USERNAME_INPUT_STRING, highScoreUsername);
            fContainerRef.drawSingleString(displayWindow, GameFontStrings::HIGHSCORE_ACHIEVED);
            fContainerRef.drawSingleString(displayWindow, GameFontStrings::ENTER_USERNAME);
            fContainerRef.drawSingleString(displayWindow, GameFontStrings::USERNAME_INPUT_STRING);
        }
        else
        {
            if (highScoreAchieved)
            {
                fContainerRef.drawSingleString(displayWindow, GameFontStrings::HIGHSCORE_ACHIEVED);
            }
            fContainerRef.drawSingleString(displayWindow, GameFontStrings::GAME_OVER_USER_SELECTION);
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

void DisplayContainer::prepeareMeatadataFile()
{
    metadataFile =
        std::getenv("HOME") + std::string(TOSTRINGYFY(META_DATA_FILE_PATH)) + std::string(MEATA_DATA_FILE_NAME);
    bool fileexist = std::filesystem::exists(metadataFile);
    if (fileexist)
    {
        metadataFileHandle.open(metadataFile, std::ios::in | std::ios::binary);
        if (metadataFileHandle.is_open())
        {
            // Read username until comma
            std::string username;
            uint8_t highscorebuffer[4];
            char ch;
            while (!metadataFileHandle.eof())
            {
                metadataFileHandle.read(&ch, sizeof(char));
                if (ch == ',')
                {
                    uint8_t highscorebuffer[4];
                    uint32_t tempHighScore = 0;
                    metadataFileHandle.read((char *)(&highscorebuffer[0]), sizeof(highscorebuffer));

                    for (unsigned int i = 0; i < 4; i++)
                    {
                        tempHighScore |= (highscorebuffer[i] << (i * 8));
                    }
                    savedHighScoreData.push_back(std::make_pair(tempHighScore, username));

                    minHighScore = std::min(tempHighScore, minHighScore);
                    maxHighScore = std::max(tempHighScore, maxHighScore);

                    std::cout << "read from file:" << username << tempHighScore << std::endl;
                    username.clear();
                }
                else if (ch == ';')
                {
                    continue;
                }
                else
                {
                    username += ch;
                }
            }
        }

        prepareHighscoreDisplaydata();

        metadataFileHandle.close();
        metadataFileHandle.open(metadataFile, std::ios::out | std::ios::binary);
    }
    else
    {
        auto dirPath = std::getenv("HOME") + std::string(TOSTRINGYFY(META_DATA_FILE_PATH));
        if (mkdir(dirPath.c_str(), 0755) != 0 && errno != EEXIST) {
            std::cerr << "Failed to create directory: " << dirPath << " - " << strerror(errno) << std::endl;
        }

        metadataFileHandle.open(metadataFile, std::ios::out | std::ios::binary);
        if (metadataFileHandle.is_open())
            std::cout << "file is created and opened" << std::endl;
    }
}

void DisplayContainer::saveHighScoreInFile()
{
    char comma = ',';
    char semicolon = ';';

    for (auto &[score, name] : savedHighScoreData)
    {
        std::cout << "writing to file:" << score << name << std::endl;
        metadataFileHandle.write(name.c_str(), name.size());
        metadataFileHandle.write(&comma, sizeof(comma));
        metadataFileHandle.write((char *)(&score), 4);
        metadataFileHandle.write(&semicolon, sizeof(semicolon));
    }
}


void DisplayContainer::prepareHighscoreDisplaydata()
{
    if (savedHighScoreData.size() > 0)
    {
        // displaying in descending order
        std::sort(savedHighScoreData.begin(), savedHighScoreData.end(),
                    [](const auto &a, const auto &b) { return a.first > b.first; });
        for (auto &[score, username] : savedHighScoreData)
        {
            highScoreDisplayData += username;
            highScoreDisplayData += ":";
            highScoreDisplayData += std::to_string(score);
            highScoreDisplayData += "\n";
        }
    }
}