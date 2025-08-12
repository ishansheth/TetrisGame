#include "FontContainer.h"

FontContainer::FontContainer()
{
    std::string fontFilePath = std::getenv("HOME") + std::string(TOSTRINGYFY(FONTS_FILE_PATH));
    gameFont.loadFromFile(fontFilePath);
}

void FontContainer::initializeFontContainer()
{
    for(std::size_t idx = 0; idx < static_cast<std::size_t>(GameFontStrings::MAXIMUM_NUMBER_OF_FONTS); idx++)
    {
        addFont(static_cast<GameFontStrings>(idx));
    }
}


void FontContainer::addFont(const GameFontStrings& fontKey)
{
    fContainer[fontKey] = sf::Text();
    fContainer[fontKey].setFont(gameFont);
    // since fontKeyToStrings and stringToLocation are const map, operator[] can not be used to access the key,value
    // instead use at funtion of std::map, which throws exception
    fContainer[fontKey].setString(fontKeyToStrings.at(fontKey));
    fContainer[fontKey].setCharacterSize(FONT_SIZE);
    fContainer[fontKey].setFillColor(sf::Color::White);
    fContainer[fontKey].setPosition(stringToLocation.at(fontKey));
}

void FontContainer::drawFonts(sf::RenderWindow &displayWindow)
{
    // these are default strings which are always displayed during game
    displayWindow.draw(fContainer[GameFontStrings::NEXT_SHAPE_LABEL]);
    displayWindow.draw(fContainer[GameFontStrings::SCORE_VALUE]);
    displayWindow.draw(fContainer[GameFontStrings::SCORE_LABEL]);
    displayWindow.draw(fContainer[GameFontStrings::STAGE_LABEL]);
    displayWindow.draw(fContainer[GameFontStrings::STAGE_VALUE]);
    displayWindow.draw(fContainer[GameFontStrings::HIGH_SCORE_BOARD_STRING]);
    displayWindow.draw(fContainer[GameFontStrings::HIGH_SCORE_VALUES]);
}

void FontContainer::drawSingleString(sf::RenderWindow &displayWindow, const GameFontStrings label)
{
    displayWindow.draw(fContainer[label]);
}

void FontContainer::drawSingleString(sf::RenderWindow &displayWindow, const GameFontStrings label, int x, int y)
{
    fContainer[label].setPosition(sf::Vector2f(x, y));
    displayWindow.draw(fContainer[label]);
}

void FontContainer::setFontString(const GameFontStrings fontkey, const std::string &strVal)
{
    fContainer[fontkey].setString(strVal);
}
