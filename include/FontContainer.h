#pragma once
#include "GameConstants.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <string_view>
#include <unordered_map>

#define STRINGYFY(x) #x
#define TOSTRINGYFY(x) STRINGYFY (x)

enum class GameFontStrings {
    NEXT_SHAPE_LABEL,
    SCORE_LABEL,
    SCORE_INITIAL_VALUE,
    GAME_OVER,
    GAME_OVER_USER_SELECTION
};

static const std::unordered_map<GameFontStrings, std::string> fontKeyToStrings{
    { GameFontStrings::NEXT_SHAPE_LABEL, "Next Shape:" },
    { GameFontStrings::SCORE_LABEL, "Score:" },
    { GameFontStrings::SCORE_INITIAL_VALUE, "0" },
    { GameFontStrings::GAME_OVER, "        GAME OVER! \n Press \"R\" to Continue, \"E\" to exit " }
};

class FontContainer {
    std::unordered_map<GameFontStrings, sf::Text> fContainer;
    sf::Font gameFont;
    std::string fontFilePath;

    public:
    FontContainer ();
    void addFont (const GameFontStrings, int x, int y);

    void displayFonts (sf::RenderWindow&);
    void setFontString (const GameFontStrings, const std::string&);
    void displaySingleString (sf::RenderWindow&, const GameFontStrings);
};