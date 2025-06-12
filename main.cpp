#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "DisplayContainer.h"
#include "FontContainer.h"
#include "StageManager.h"
#include <vector>

int main () {

    sf::RenderWindow displayWindow (sf::VideoMode (WINDOW_WIDTH, WINDOW_HEIGHT), "Tetris");

    FontContainer fontContainer;
    fontContainer.addFont (
    GameFontStrings::NEXT_SHAPE_LABEL, FONT_NEXT_SHAPE_X, FONT_NEXT_SHAPE_Y);
    fontContainer.addFont (GameFontStrings::SCORE_LABEL, FONT_SCORE_X, FONT_SCORE_Y);
    fontContainer.addFont (GameFontStrings::SCORE_INITIAL_VALUE,
    FONT_SCORE_VALUE_X, FONT_SCORE_VALUE_Y);
    fontContainer.addFont (GameFontStrings::GAME_OVER, FONT_GAMEOVER_X, FONT_GAMEOVER_Y);
    ShapeGenerator shapegen;
    DisplayContainer displayContainer (fontContainer, shapegen);

    StageManager sMgr(fontContainer, displayWindow);
    bool gamePause = false;

    while (displayWindow.isOpen ()) {
        sf::Event event;

        while (displayWindow.pollEvent (event)) {
            if (event.type == sf::Event::Closed)
                displayWindow.close ();
        }
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Space)) {
            std::this_thread::sleep_for (std::chrono::milliseconds (90));
            if (sf::Keyboard::isKeyPressed (sf::Keyboard::Space)) {
                displayContainer.handleKey (sf::Keyboard::Space);
            }
        } else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Left)) {
            std::this_thread::sleep_for (std::chrono::milliseconds (90));
            if (sf::Keyboard::isKeyPressed (sf::Keyboard::Left)) {
                displayContainer.handleKey (sf::Keyboard::Left);
            }
        } else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Right)) {
            std::this_thread::sleep_for (std::chrono::milliseconds (90));
            if (sf::Keyboard::isKeyPressed (sf::Keyboard::Right)) {
                //sMgr.handleUserInput (sf::Keyboard::Right);
                displayContainer.handleKey(sf::Keyboard::Right);
            }
        } else if (sf::Keyboard::isKeyPressed (sf::Keyboard::P)) {
            gamePause = true;
        } else if (sf::Keyboard::isKeyPressed (sf::Keyboard::R)) {
            gamePause = false;
        }

        // 1. check stage para
        // 2. generate stage shapes
        // 3. process stage
        // sMgr.checkStageStatus();
        // sMgr.generateStageScreen();
        // sMgr.processStageShapes();

        if(!gamePause)
        {
            displayContainer.generateAndDrawShape (displayWindow);
        }

        displayContainer.processshapes ();

        displayContainer.handleGameState (displayWindow);
    }

    return 0;
}