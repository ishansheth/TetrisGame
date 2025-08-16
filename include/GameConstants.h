#pragma once

#define STRINGYFY(x) #x
#define TOSTRINGYFY(x) STRINGYFY(x)

///////////////////////////////////////////////////// Window and shape parameter const
/////////////////////////////////////
static constexpr unsigned int SQUARE_SIDE_LENGTH = 20;
static constexpr unsigned int BOMB_SQUARE_SIDE_LENGTH = 40;

static constexpr unsigned int SQUARE_OUTLINE_THICKNESS = 2;
static constexpr unsigned int HALF_SQUARE_LENGTH = 13;

static constexpr unsigned int SQUARE_SIDE_LENGTH_WITH_OUTLINE = (SQUARE_SIDE_LENGTH + (2 * SQUARE_OUTLINE_THICKNESS));
static constexpr unsigned int NUMBER_OF_SQUARES_IN_ROW = 20;

static constexpr unsigned int DRAW_WINDOW_WIDTH = SQUARE_SIDE_LENGTH_WITH_OUTLINE * NUMBER_OF_SQUARES_IN_ROW;
static constexpr unsigned int DRAW_WINDOW_HEIGHT = 600;
static constexpr float SHAPE_DOWN_FALL_SPEED_X = 0.f;

// this is not a constant, it value changes
static float SHAPE_DOWN_FALL_SPEED_Y = 0.3f;

static constexpr unsigned int NUMBER_OF_ROWS_IN_GAME = 23;

static constexpr unsigned int NEXT_SHAPE_X = DRAW_WINDOW_WIDTH + 10;
static constexpr unsigned int NEXT_SHAPE_Y = 50;

static constexpr unsigned int LAST_ROW_Y = DRAW_WINDOW_HEIGHT - SQUARE_SIDE_LENGTH - 2 * SQUARE_OUTLINE_THICKNESS;

static constexpr unsigned int NUMBER_OF_STAGE_IN_GAME = 3;

static constexpr unsigned int SHAPE_COUNT_FOR_STAGE[NUMBER_OF_STAGE_IN_GAME] = {4, 5, 6};

///////////////////////////////////////////////////// Font related const
///////////////////////////////////////////////////
static constexpr unsigned int FONT_SIZE = 24;

static constexpr unsigned int STAGE_COMPLETE_MSG_X = 250;
static constexpr unsigned int STAGE_COMPLETE_MSG_Y = 200;

static constexpr unsigned int SCORE_PER_ROW = 10;

// static constexpr unsigned int CLEARING_SCORE_PER_STAGE[NUMBER_OF_STAGE_IN_GAME] = {300, 900, 2700};

static constexpr unsigned int CLEARING_SCORE_PER_STAGE[NUMBER_OF_STAGE_IN_GAME] = {50, 100, 150};

static constexpr unsigned int NO_ROWS_DESROYED_BY_BOMB = 3;

static constexpr unsigned int NO_SAVED_HIGHSCORE_USERNAMES = 5; 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const char *BLIP_SOUND_FILE_NAME = "hitHurt.wav";
static const char *EXPLOSION_SOUND_FILE_NAME = "explosion.wav";
static const char *BOMB_EXPLOSION_SOUND_FILE_NAME = "bomb_explosion.wav";
static const char* MEATA_DATA_FILE_NAME = "tetrismetadata.bin";

