#include "Util.h"
#include "GameConstants.h"

bool isWithinWindow (sf::Vector2f v) {
    return (v.x - SQUARE_SIDE_LENGTH / 2 + SQUARE_OUTLINE_THICKNESS > 0 &&
    v.x + SQUARE_SIDE_LENGTH / 2 + SQUARE_OUTLINE_THICKNESS < WINDOW_WIDTH &&
    v.y - SQUARE_SIDE_LENGTH / 2 + SQUARE_OUTLINE_THICKNESS > 0 &&
    v.y + SQUARE_SIDE_LENGTH / 2 + SQUARE_OUTLINE_THICKNESS < WINDOW_HEIGHT);
}

bool isWithinDrawWindow (sf::Vector2f v) {
    return (v.x >= 0 && v.x + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS <= DRAW_WINDOW_WIDTH &&
    v.y >= 0 && v.y + SQUARE_SIDE_LENGTH + SQUARE_OUTLINE_THICKNESS <= DRAW_WINDOW_HEIGHT);
}
