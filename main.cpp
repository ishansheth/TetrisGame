#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "DisplayContainer.h"
#include "FontContainer.h"
#include "StageManager.h"
#include <vector>
#include "MetaFileHandler.h"

#include <csignal>

// Signal handler function
void handleSignal(int signal) 
{
    MetaFileHandler::saveMetaDataFileAndClose();
    exit(signal);
}

// Call this function at the start of main() or before your game loop
void setupSignalHandlers() 
{
    std::signal(SIGINT, handleSignal);  // Ctrl+C
    std::signal(SIGTERM, handleSignal); // kill command
}

int main () 
{
    setupSignalHandlers();
    sf::RenderWindow displayWindow (sf::VideoMode (WINDOW_WIDTH, WINDOW_HEIGHT), "Tetris");

    FontContainer fontContainer;
    fontContainer.initializeFontContainer();
    
    ShapeGenerator shapegen;
    DisplayContainer displayContainer (fontContainer, shapegen);

    // StageManager sMgr(fontContainer, displayWindow);
    bool gamePause = false;

    displayContainer.showCurrentStageScreen(displayWindow);

    while (displayWindow.isOpen ()) {
        sf::Event event;

        while (displayWindow.pollEvent (event)) {
            if (event.type == sf::Event::Closed)
            {
                displayContainer.cleanDisplayContainer();
                MetaFileHandler::saveMetaDataFileAndClose();
                displayWindow.close ();
            }
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

        displayContainer.processshapes (displayWindow);

    }

    return 0;
}