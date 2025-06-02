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

    DisplayContainer displayContainer (fontContainer);

    StageManager sMgr;

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
                displayContainer.handleKey (sf::Keyboard::Right);
            }
        }

        sMgr.generateStageShapes ();
        sMgr.processShapes ();
        sMgr.processStage ();

        displayContainer.generateAndDrawShape (displayWindow);

        displayContainer.processshapes ();

        displayContainer.handleGameState (displayWindow);
    }

    return 0;
}