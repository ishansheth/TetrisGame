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
    fontContainer.addFont (GameFontStrings::SCORE_VALUE, FONT_SCORE_VALUE_X, FONT_SCORE_VALUE_Y);
    fontContainer.addFont (GameFontStrings::GAME_OVER, FONT_GAMEOVER_X, FONT_GAMEOVER_Y);
    fontContainer.addFont (GameFontStrings::GAME_PAUSED, GAME_PAUSED_X, GAME_PAUSED_Y);
    fontContainer.addFont (GameFontStrings::STAGE_LABEL, STAGE_LEBEL_X, STAGE_LEBEL_Y);
    fontContainer.addFont (GameFontStrings::STAGE_VALUE, STAGE_VALUE_X, STAGE_VALUE_Y);
    fontContainer.addFont (GameFontStrings::STAGE_COMPLETE_MESSAGE, STAGE_COMPLETE_MSG_X, STAGE_COMPLETE_MSG_Y);

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
            if (sf::Keyboard::isKeyPressed (sf::Keyboard::Space) && !gamePause) {                
                displayContainer.handleKey (sf::Keyboard::Space);
            }
        } else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Left)) {
            std::this_thread::sleep_for (std::chrono::milliseconds (90));
            if (sf::Keyboard::isKeyPressed (sf::Keyboard::Left) && !gamePause) {
                displayContainer.handleKey (sf::Keyboard::Left);
            }
        } else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Right)) {
            std::this_thread::sleep_for (std::chrono::milliseconds (90));
            if (sf::Keyboard::isKeyPressed (sf::Keyboard::Right) && !gamePause) {
                //sMgr.handleUserInput (sf::Keyboard::Right);
                displayContainer.handleKey(sf::Keyboard::Right);
            }
        } else if (sf::Keyboard::isKeyPressed (sf::Keyboard::P)) {
            displayContainer.setGamePaused();
        } else if (sf::Keyboard::isKeyPressed (sf::Keyboard::R)) {
            displayContainer.resetGamePaused();
        }

        // 1. check stage para
        // 2. generate stage shapes
        // 3. process stage
        // sMgr.checkStageStatus();
        // sMgr.generateStageScreen();
        // sMgr.processStageShapes();
        displayContainer.generateAndDrawShape (displayWindow);

        displayContainer.handleGameState (displayWindow);

        displayContainer.processshapes ();

    }

    return 0;
}