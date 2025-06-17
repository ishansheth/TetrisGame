#include "FontContainer.h"

FontContainer::FontContainer () {
    std::string fontFilePath =
    std::getenv ("HOME") + std::string (TOSTRINGYFY (FONT_FILE_PATH));
    gameFont.loadFromFile (fontFilePath);
}

void FontContainer::addFont (const GameFontStrings fontKey, int x, int y) {
    fContainer[fontKey] = sf::Text ();
    fContainer[fontKey].setFont (gameFont);
    fContainer[fontKey].setString (fontKeyToStrings.at (fontKey));
    fContainer[fontKey].setCharacterSize (FONT_SIZE);
    fContainer[fontKey].setFillColor (sf::Color::White);
    fContainer[fontKey].setPosition (sf::Vector2f (x, y));
}

void FontContainer::drawFonts (sf::RenderWindow& displayWindow) {
    displayWindow.draw (fContainer[GameFontStrings::NEXT_SHAPE_LABEL]);
    displayWindow.draw (fContainer[GameFontStrings::SCORE_VALUE]);
    displayWindow.draw (fContainer[GameFontStrings::SCORE_LABEL]);
    displayWindow.draw (fContainer[GameFontStrings::STAGE_LABEL]);
    displayWindow.draw (fContainer[GameFontStrings::STAGE_VALUE]);
}

void FontContainer::drawSingleString (sf::RenderWindow& displayWindow,
const GameFontStrings label) {
    displayWindow.draw (fContainer[label]);
}

void FontContainer::drawSingleString (sf::RenderWindow& displayWindow, const GameFontStrings label,int x, int y) {
    fContainer[label].setPosition (sf::Vector2f (x, y));
    displayWindow.draw (fContainer[label]);
}

void FontContainer::setFontString (const GameFontStrings fontkey, const std::string& strVal) {
    fContainer[fontkey].setString (strVal);
}
