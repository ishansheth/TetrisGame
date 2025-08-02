#include "DisplayContainer.h"
#include "Util.h"
#include <iostream>
#include <thread>

DisplayContainer::DisplayContainer(FontContainer &fCon, ShapeGenerator &shapegenerator)
    : shapeGen(shapegenerator), lastShape(nullptr), nextShape(nullptr), moveStatus(true), scoreValue(0),
      isGameOverState(false), isGamePaused(false), fContainerRef(fCon), currentStageNumber(1), xCoordinateEmitter(-100),
      bombExplosionParticles(1000)
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
}

bool DisplayContainer::isGameOver()
{
    return (individualComponentContainer[FIRST_ROW_Y].size() > 0);
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

int DisplayContainer::getScore()
{
    return scoreValue;
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

    fContainerRef.setFontString(GameFontStrings::SCORE_VALUE, std::to_string(getScore()));
    fContainerRef.setFontString(GameFontStrings::STAGE_VALUE, std::to_string(currentStageNumber));
}

void DisplayContainer::handleBombDrop(sf::RenderWindow &displayWindow)
{
    float minx = static_cast<int>((*(lastShape->getShapeContianer()[0]))->getPosition().x);
    float maxx = minx + (3*SQUARE_SIDE_LENGTH_WITH_OUTLINE);

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

    if(removeFromRows.size() == 0)
    {
        return;
    }

    bombExplosionParticles.setEmitter(sf::Vector2f((maxx + minx)/2, removeFromRows[0]));

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

    moveStatus = lastShape->getMoveStatus();
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
    bool shiftRequired = false;

    eraseCompletedRow(yVal,displayWindow);
    scoreValue += SCORE_PER_ROW;
    shiftRequired = true;

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
        isGameOverState = true;
        scoreValue = 0;
        cleanDisplayContainer();
        fContainerRef.drawSingleString(displayWindow, GameFontStrings::GAME_OVER);
        sf::Event event;

        while (displayWindow.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                {
                    isGameOverState = false;
                    generateAndDrawShape(displayWindow);
                    break;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                {
                    displayWindow.close();
                    break;
                }
            }
        }
    }
    else if (isGamePaused)
    {
        fContainerRef.drawSingleString(displayWindow, GameFontStrings::GAME_PAUSED);
        fContainerRef.drawFonts(displayWindow);
        displayWindow.draw(partitionLine, 2, sf::Lines);
    }
    else if (getScore() >= CLEARING_SCORE_PER_STAGE[currentStageNumber - 1])
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
    if (currentStageNumber == 1)
    {
        SHAPE_DOWN_FALL_SPEED_Y = 0.3f;
        shapeGen.setAllowedShapesCount(SHAPE_COUNT_FOR_STAGE[currentStageNumber - 1]);
    }
    else if (currentStageNumber == 2)
    {
        // set allowed shapes
        SHAPE_DOWN_FALL_SPEED_Y = 0.2f;
        shapeGen.setAllowedShapesCount(SHAPE_COUNT_FOR_STAGE[currentStageNumber - 1]);
    }
    else if (currentStageNumber == 3)
    {
        // set allowed shapes
        SHAPE_DOWN_FALL_SPEED_Y = 0.3f;
        shapeGen.setAllowedShapesCount(SHAPE_COUNT_FOR_STAGE[currentStageNumber - 1]);
    }
    else if (currentStageNumber == 4)
    {
        // set allowed shapes
        SHAPE_DOWN_FALL_SPEED_Y = 0.7f;
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
    fContainerRef.drawSingleString(displayWindow, GameFontStrings::STAGE_LABEL, STAGE_LEBEL_X, STAGE_LEBEL_Y);
    fContainerRef.drawSingleString(displayWindow, GameFontStrings::STAGE_VALUE, STAGE_VALUE_X, STAGE_VALUE_Y);
}

void DisplayContainer::cleanDisplayContainer()
{
    for (auto it = individualComponentContainer.begin(); it != individualComponentContainer.end(); it++)
    {
        for (auto &element : it->second)
        {
            if (element.first != nullptr)
                delete *element.first;
            *element.first = nullptr;
        }
        it->second.clear();
    }
}
