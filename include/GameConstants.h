#pragma once

///////////////////////////////////////////////////// Window and shape parameter const //////////////////////////////////
static constexpr unsigned int SQUARE_SIDE_LENGTH       = 20;
static constexpr unsigned int SQUARE_OUTLINE_THICKNESS = 2;
static constexpr unsigned int HALF_SQUARE_LENGTH       = 13;

static constexpr unsigned int SQUARE_SIDE_LENGTH_WITH_OUTLINE = (SQUARE_SIDE_LENGTH + (2*SQUARE_OUTLINE_THICKNESS));
static constexpr unsigned int NUMBER_OF_SQUARES_IN_ROW = 20;

static constexpr unsigned int DRAW_WINDOW_WIDTH  = SQUARE_SIDE_LENGTH_WITH_OUTLINE*NUMBER_OF_SQUARES_IN_ROW;
static constexpr unsigned int DRAW_WINDOW_HEIGHT = 600;
static constexpr float SHAPE_DOWN_FALL_SPEED_X   = 0.f;
static constexpr float SHAPE_DOWN_FALL_SPEED_Y   = 0.3f;

static constexpr unsigned int NUMBER_OF_ROWS_IN_GAME = 22;

static constexpr unsigned int NEXT_SHAPE_X = DRAW_WINDOW_WIDTH + 10;
static constexpr unsigned int NEXT_SHAPE_Y = 50;

static constexpr unsigned int FIRST_ROW_Y = 4;
static constexpr unsigned int LAST_ROW_Y =
DRAW_WINDOW_HEIGHT - SQUARE_SIDE_LENGTH - 2*SQUARE_OUTLINE_THICKNESS;

///////////////////////////////////////////////////// Font related const ////////////////////////////////////////////////
static constexpr unsigned int FONT_SIZE = 24;

static constexpr unsigned int FONT_NEXT_SHAPE_X = DRAW_WINDOW_WIDTH + 10;
static constexpr unsigned int FONT_NEXT_SHAPE_Y = 10;

static constexpr unsigned int FONT_SCORE_X = DRAW_WINDOW_WIDTH + 10;
static constexpr unsigned int FONT_SCORE_Y = 130;

static constexpr unsigned int FONT_SCORE_VALUE_X = DRAW_WINDOW_WIDTH + 10;
static constexpr unsigned int FONT_SCORE_VALUE_Y = 160;

static constexpr unsigned int FONT_GAMEOVER_X = 250;
static constexpr unsigned int FONT_GAMEOVER_Y = 200;

static constexpr unsigned int GAME_PAUSED_X = 250;
static constexpr unsigned int GAME_PAUSED_Y = 200;

static constexpr unsigned int SCORE_PER_ROW = 10;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
