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
    STAGE_COMPLETE_MESSAGE,
    HIGH_SCORE_BOARD_STRING,
    HIGH_SCORE_VALUES,
    USERNAME_INPUT_STRING,
    HIGHSCORE_ACHIEVED,
    ENTER_USERNAME,
    MAXIMUM_NUMBER_OF_FONTS
};

static const std::unordered_map<GameFontStrings, std::string> fontKeyToStrings{
    {GameFontStrings::NEXT_SHAPE_LABEL, "Next Shape:"},
    {GameFontStrings::SCORE_LABEL, "Score:"},
    {GameFontStrings::SCORE_VALUE, "0"},
    {GameFontStrings::GAME_OVER, "GAME OVER!"},
    {GameFontStrings::GAME_OVER_USER_SELECTION, "Press \"F10\" to Continue, \"Esc\" to exit "},
    {GameFontStrings::HIGHSCORE_ACHIEVED, "New HighScore Achieved!"},
    {GameFontStrings::ENTER_USERNAME, "Enter Name:"},
    {GameFontStrings::USERNAME_INPUT_STRING, ""},    
    {GameFontStrings::GAME_PAUSED, "Paused"},
    {GameFontStrings::STAGE_LABEL, "Stage:"},
    {GameFontStrings::STAGE_VALUE, ""},
    {GameFontStrings::HIGH_SCORE_BOARD_STRING, "High Score Board"},
    {GameFontStrings::HIGH_SCORE_VALUES, ""},
    {GameFontStrings::STAGE_COMPLETE_MESSAGE, ""}
};

static const std::unordered_map<GameFontStrings, sf::Vector2f> stringToLocation{
    {GameFontStrings::NEXT_SHAPE_LABEL, {DRAW_WINDOW_WIDTH + 10, 10}},
    {GameFontStrings::SCORE_LABEL, {DRAW_WINDOW_WIDTH + 10, 130}},
    {GameFontStrings::SCORE_VALUE, {DRAW_WINDOW_WIDTH + 80, 130}},
    {GameFontStrings::GAME_OVER, {250,200}},

    {GameFontStrings::HIGHSCORE_ACHIEVED, {250,240}},
    {GameFontStrings::ENTER_USERNAME, {250,270}},
    {GameFontStrings::USERNAME_INPUT_STRING, {390,270}},

    {GameFontStrings::GAME_OVER_USER_SELECTION, {250,300}},
    {GameFontStrings::GAME_PAUSED, {250,200}},
    {GameFontStrings::STAGE_LABEL, {DRAW_WINDOW_WIDTH + 10, 170}},
    {GameFontStrings::STAGE_VALUE, {DRAW_WINDOW_WIDTH + 80, 170}},
    {GameFontStrings::HIGH_SCORE_BOARD_STRING, {DRAW_WINDOW_WIDTH + 10, 250}},
    {GameFontStrings::HIGH_SCORE_VALUES, {DRAW_WINDOW_WIDTH + 10, 285}},
    {GameFontStrings::STAGE_COMPLETE_MESSAGE, {250,200}}
};

class FontContainer
{
    std::unordered_map<GameFontStrings, sf::Text> fContainer;
    sf::Font gameFont;
    std::string fontFilePath;

    void addFont(const GameFontStrings&);

  public:
    FontContainer();
    void initializeFontContainer();
    void drawFonts(sf::RenderWindow &);
    void setFontString(const GameFontStrings, const std::string &);
    void drawSingleString(sf::RenderWindow &, const GameFontStrings);
    void drawSingleString(sf::RenderWindow &displayWindow, const GameFontStrings label, int x, int y);
};