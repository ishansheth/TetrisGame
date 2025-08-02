#pragma once
#include "GameConstants.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <string_view>
#include <unordered_map>


enum class GameFontStrings
{
    NEXT_SHAPE_LABEL,
    SCORE_LABEL,
    SCORE_VALUE,
    GAME_OVER,
    GAME_OVER_USER_SELECTION,
    GAME_PAUSED,
    STAGE_LABEL,
    STAGE_VALUE,
    STAGE_COMPLETE_MESSAGE
};

static const std::unordered_map<GameFontStrings, std::string> fontKeyToStrings{
    {GameFontStrings::NEXT_SHAPE_LABEL, "Next Shape:"},
    {GameFontStrings::SCORE_LABEL, "Score:"},
    {GameFontStrings::SCORE_VALUE, "0"},
    {GameFontStrings::GAME_OVER, "        GAME OVER! \n Press \"R\" to Continue, \"E\" to exit "},
    {GameFontStrings::GAME_PAUSED, "Paused"},
    {GameFontStrings::STAGE_LABEL, "Stage:"},
    {GameFontStrings::STAGE_VALUE, ""},
    {GameFontStrings::STAGE_COMPLETE_MESSAGE, ""}};

class FontContainer
{
    std::unordered_map<GameFontStrings, sf::Text> fContainer;
    sf::Font gameFont;
    std::string fontFilePath;

  public:
    FontContainer();
    void addFont(const GameFontStrings, int x, int y);

    void drawFonts(sf::RenderWindow &);
    void setFontString(const GameFontStrings, const std::string &);
    void drawSingleString(sf::RenderWindow &, const GameFontStrings);
    void drawSingleString(sf::RenderWindow &displayWindow, const GameFontStrings label, int x, int y);
};