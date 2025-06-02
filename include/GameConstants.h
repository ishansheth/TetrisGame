#pragma once

///////////////////////////////////////////////////// Window and shape parameter const //////////////////////////////////
static constexpr unsigned int SQUARE_SIDE_LENGTH       = 20;
static constexpr unsigned int SQUARE_OUTLINE_THICKNESS = 3;
static constexpr unsigned int HALF_SQUARE_LENGTH       = 13;

static constexpr unsigned int DRAW_WINDOW_WIDTH  = 520;
static constexpr unsigned int DRAW_WINDOW_HEIGHT = 600;
static constexpr float SHAPE_DOWN_FALL_SPEED_X   = 0.f;
static constexpr float SHAPE_DOWN_FALL_SPEED_Y   = 1.f;

static constexpr unsigned int NEXT_SHAPE_X = DRAW_WINDOW_WIDTH + 10;
static constexpr unsigned int NEXT_SHAPE_Y = 50;

static constexpr unsigned int FIRST_ROW_Y = 5;
static constexpr unsigned int LAST_ROW_Y =
WINDOW_HEIGHT - SQUARE_SIDE_LENGTH - SQUARE_OUTLINE_THICKNESS;

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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
