#include "DisplayContainer.h"
#include <iostream>

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
}

void DisplayContainer::processshapes () {
    if (isGameOverState) {
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

    std::string displayScoreVal = std::to_string (getScore ());
    fContainerRef.setFontString (GameFontStrings::SCORE_INITIAL_VALUE, displayScoreVal);
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
        fContainerRef.displaySingleString (displayWindow, GameFontStrings::GAME_OVER);
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
        fContainerRef.displaySingleString (displayWindow, GameFontStrings::GAME_PAUSED);
        fContainerRef.displayFonts (displayWindow);
        displayWindow.draw (partitionLine, 2, sf::Lines);
    }
    else if(!isGamePaused && !isGameOverState)
    {
        moveShapes();
        fContainerRef.displayFonts (displayWindow);
        displayWindow.draw (partitionLine, 2, sf::Lines);
    }


    // for (std::size_t i = 0; i < getGridLines().size(); i++)
    // {
    //     displayWindow.draw(getGridLines()[i].data(), 2, sf::Lines);
    // }

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
