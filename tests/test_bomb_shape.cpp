// Tests for BombShape changes introduced in this PR:
//
//   The PR removed the isWithinDrawWindow() and isIntersecting() guards from
//   BombShape::moveShape(), so the bomb now always moves and always returns true.
//   Previously, the bomb would stop (isMoving = false) when it hit a boundary
//   or an intersection.
//
//   Key behavioural change:
//     Before: BombShape::moveShape() could return false and set isMoving=false.
//     After:  BombShape::moveShape() always returns true (unconditional move).

#include <gtest/gtest.h>

// Provide the extern float required by shape constructors.
float SHAPE_DOWN_FALL_SPEED_Y = 0.1f;

#include "BombShape.h"

namespace {

// BombShape constructor requires a DisplayContainer* but does not dereference
// it during construction, so nullptr is safe for these tests.
constexpr DisplayContainer* kNullDC = nullptr;

// ────────────────────────────────────────────────────────────────────────────
// Basic identity
// ────────────────────────────────────────────────────────────────────────────

TEST(BombShapeIdentity, IsBomb_ReturnsTrue) {
    BombShape bomb(kNullDC);
    EXPECT_TRUE(bomb.isBomb());
}

TEST(BombShapeIdentity, IsShapeBroken_InitiallyFalse) {
    BombShape bomb(kNullDC);
    EXPECT_FALSE(bomb.isShapeBroken());
}

TEST(BombShapeIdentity, GetMoveStatus_InitiallyTrue) {
    BombShape bomb(kNullDC);
    // After construction the bomb should be moving.
    EXPECT_TRUE(bomb.getMoveStatus());
}

TEST(BombShapeIdentity, GetShapeContainer_HasExactlyOneRectangle) {
    BombShape bomb(kNullDC);
    EXPECT_EQ(bomb.getShapeContainer().size(), 1u);
}

// ────────────────────────────────────────────────────────────────────────────
// moveShape() – always returns true (PR change)
// ────────────────────────────────────────────────────────────────────────────

TEST(BombShapeMoveShape, MoveShape_AlwaysReturnsTrue_FirstCall) {
    BombShape bomb(kNullDC);
    bomb.setPosition(sf::Vector2f{50.f, 50.f});
    EXPECT_TRUE(bomb.moveShape());
}

TEST(BombShapeMoveShape, MoveShape_AlwaysReturnsTrue_MultipleConsecutiveCalls) {
    BombShape bomb(kNullDC);
    bomb.setPosition(sf::Vector2f{50.f, 50.f});
    // Call several times – all must return true.
    for (int i = 0; i < 10; ++i) {
        EXPECT_TRUE(bomb.moveShape()) << "moveShape() returned false on call " << i + 1;
    }
}

// Before the PR, the bomb would stop once it went past DRAW_WINDOW_HEIGHT.
// After the PR, moveShape() still returns true regardless of y position.
TEST(BombShapeMoveShape, MoveShape_ReturnsTrueEvenBeyondWindowBoundary) {
    BombShape bomb(kNullDC);
    // Place the bomb well below the visible draw window height (600).
    bomb.setPosition(sf::Vector2f{50.f, static_cast<float>(DRAW_WINDOW_HEIGHT + 100)});
    EXPECT_TRUE(bomb.moveShape());
}

// getMoveStatus() should still be true after moveShape() calls (no longer set to false).
TEST(BombShapeMoveShape, GetMoveStatus_RemainsTrue_AfterMoveShape) {
    BombShape bomb(kNullDC);
    bomb.setPosition(sf::Vector2f{50.f, 50.f});
    bomb.moveShape();
    bomb.moveShape();
    EXPECT_TRUE(bomb.getMoveStatus());
}

// ────────────────────────────────────────────────────────────────────────────
// moveShape() – position actually changes
// ────────────────────────────────────────────────────────────────────────────

TEST(BombShapeMoveShape, MoveShape_UpdatesPosition) {
    BombShape bomb(kNullDC);
    bomb.setPosition(sf::Vector2f{50.f, 50.f});

    const auto* rect = *(bomb.getShapeContainer()[0]);
    const float yBefore = rect->getPosition().y;

    bomb.moveShape();

    const float yAfter = rect->getPosition().y;
    // The bomb must have moved downward by SHAPE_DOWN_FALL_SPEED_Y.
    EXPECT_NEAR(yAfter - yBefore, SHAPE_DOWN_FALL_SPEED_Y, 1e-4f);
}

TEST(BombShapeMoveShape, MoveShape_XPositionUnchanged) {
    BombShape bomb(kNullDC);
    bomb.setPosition(sf::Vector2f{100.f, 50.f});

    const auto* rect = *(bomb.getShapeContainer()[0]);
    const float xBefore = rect->getPosition().x;

    bomb.moveShape();

    EXPECT_FLOAT_EQ(rect->getPosition().x, xBefore);
}

// ────────────────────────────────────────────────────────────────────────────
// canShapeFall – inherited from BaseShape, should be true
// ────────────────────────────────────────────────────────────────────────────

TEST(BombShapeCanFall, CanShapeFall_ReturnsTrue) {
    BombShape bomb(kNullDC);
    EXPECT_TRUE(bomb.canShapeFall());
}

// ────────────────────────────────────────────────────────────────────────────
// Regression: setBroken does not affect isBomb
// ────────────────────────────────────────────────────────────────────────────

TEST(BombShapeRegression, IsBomb_StillTrueAfterSetBroken) {
    BombShape bomb(kNullDC);
    bomb.setBroken();
    EXPECT_TRUE(bomb.isBomb());
    EXPECT_TRUE(bomb.isShapeBroken());
}

} // namespace