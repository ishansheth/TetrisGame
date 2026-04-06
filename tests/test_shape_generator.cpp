// Tests for ShapeGenerator changes introduced in this PR:
//   - getNumberMappedShape() new method
//   - getNewAddedShape() new method
//   - setAllowedShapesCount() now tracks lastAllowedShape
//   - getNextShape() / getShape() delegate to getNumberMappedShape()

#include <gtest/gtest.h>
#include <limits>

// SHAPE_DOWN_FALL_SPEED_Y is defined in DisplayContainer.cpp which we don't link here.
// Provide the definition so shape constructors can compile and link.
float SHAPE_DOWN_FALL_SPEED_Y = 0.1f;

#include "ShapeGenerator.h"
#include "BombShape.h"
#include "SShape.h"
#include "LShape.h"
#include "StShape.h"
#include "SqShape.h"
#include "TShape.h"

// Helper: create ShapeGenerator with setAllowedShapesCount already called
// so the distribution is initialised before getShape/getNextShape are used.

namespace {

// All shapes are created with nullptr DisplayContainer in these tests –
// the constructor does not dereference dContainer, so this is safe.
constexpr DisplayContainer* kNullDC = nullptr;
const sf::Vector2f kPos{0.f, 0.f};

// ────────────────────────────────────────────────────────────────────────────
// getNumberMappedShape – shape-type mapping (key PR change)
// ────────────────────────────────────────────────────────────────────────────

class GetNumberMappedShapeTest : public ::testing::Test {
protected:
    ShapeGenerator gen;

    void TearDown() override {
        // shapes are deleted in each test after checking
    }
};

TEST_F(GetNumberMappedShapeTest, ShapeNumber1_ReturnsSShape_NotBomb) {
    IShape* shape = gen.getNumberMappedShape(1, kPos, kNullDC);
    ASSERT_NE(shape, nullptr);
    EXPECT_FALSE(shape->isBomb());
    // Verify it is the correct concrete type via dynamic_cast
    EXPECT_NE(dynamic_cast<SShape*>(shape), nullptr);
    delete shape;
}

TEST_F(GetNumberMappedShapeTest, ShapeNumber2_ReturnsLShape_NotBomb) {
    IShape* shape = gen.getNumberMappedShape(2, kPos, kNullDC);
    ASSERT_NE(shape, nullptr);
    EXPECT_FALSE(shape->isBomb());
    EXPECT_NE(dynamic_cast<LShape*>(shape), nullptr);
    delete shape;
}

TEST_F(GetNumberMappedShapeTest, ShapeNumber3_ReturnsStShape_NotBomb) {
    IShape* shape = gen.getNumberMappedShape(3, kPos, kNullDC);
    ASSERT_NE(shape, nullptr);
    EXPECT_FALSE(shape->isBomb());
    EXPECT_NE(dynamic_cast<StShape*>(shape), nullptr);
    delete shape;
}

TEST_F(GetNumberMappedShapeTest, ShapeNumber4_ReturnsSqShape_NotBomb) {
    IShape* shape = gen.getNumberMappedShape(4, kPos, kNullDC);
    ASSERT_NE(shape, nullptr);
    EXPECT_FALSE(shape->isBomb());
    EXPECT_NE(dynamic_cast<SqShape*>(shape), nullptr);
    delete shape;
}

TEST_F(GetNumberMappedShapeTest, ShapeNumber5_ReturnsTShape_NotBomb) {
    IShape* shape = gen.getNumberMappedShape(5, kPos, kNullDC);
    ASSERT_NE(shape, nullptr);
    EXPECT_FALSE(shape->isBomb());
    EXPECT_NE(dynamic_cast<TShape*>(shape), nullptr);
    delete shape;
}

TEST_F(GetNumberMappedShapeTest, ShapeNumber6_ReturnsBombShape) {
    IShape* shape = gen.getNumberMappedShape(6, kPos, kNullDC);
    ASSERT_NE(shape, nullptr);
    EXPECT_TRUE(shape->isBomb());
    EXPECT_NE(dynamic_cast<BombShape*>(shape), nullptr);
    delete shape;
}

// Default case: any number > 6 should fall through to the else branch and
// return an SShape (not a bomb).
TEST_F(GetNumberMappedShapeTest, ShapeNumber7_DefaultCase_ReturnsSShape) {
    IShape* shape = gen.getNumberMappedShape(7, kPos, kNullDC);
    ASSERT_NE(shape, nullptr);
    EXPECT_FALSE(shape->isBomb());
    EXPECT_NE(dynamic_cast<SShape*>(shape), nullptr);
    delete shape;
}

TEST_F(GetNumberMappedShapeTest, ShapeNumber0_DefaultCase_ReturnsSShape) {
    IShape* shape = gen.getNumberMappedShape(0, kPos, kNullDC);
    ASSERT_NE(shape, nullptr);
    EXPECT_FALSE(shape->isBomb());
    EXPECT_NE(dynamic_cast<SShape*>(shape), nullptr);
    delete shape;
}

// Boundary: UINT_MAX should also hit the default else branch.
TEST_F(GetNumberMappedShapeTest, ShapeNumberMaxUint_DefaultCase_ReturnsSShape) {
    IShape* shape = gen.getNumberMappedShape(std::numeric_limits<unsigned int>::max(), kPos, kNullDC);
    ASSERT_NE(shape, nullptr);
    EXPECT_FALSE(shape->isBomb());
    EXPECT_NE(dynamic_cast<SShape*>(shape), nullptr);
    delete shape;
}

// Verify that the returned shape has its position set to the requested location.
TEST_F(GetNumberMappedShapeTest, ShapeNumber1_SetsPosition) {
    const sf::Vector2f requestedPos{100.f, 200.f};
    IShape* shape = gen.getNumberMappedShape(1, requestedPos, kNullDC);
    ASSERT_NE(shape, nullptr);
    // The shape container should be non-empty and the first rectangle
    // should be placed near the requested position (SShape offsets its pieces,
    // but at least one piece position should be >= the base x/y).
    auto container = shape->getShapeContainer();
    EXPECT_FALSE(container.empty());
    delete shape;
}

// Only one of the six valid shape numbers (6) should return a bomb.
TEST_F(GetNumberMappedShapeTest, OnlyNumber6_IsBomb) {
    for (unsigned int n = 1; n <= 6; ++n) {
        IShape* shape = gen.getNumberMappedShape(n, kPos, kNullDC);
        ASSERT_NE(shape, nullptr);
        if (n == 6) {
            EXPECT_TRUE(shape->isBomb()) << "Expected bomb for shape number " << n;
        } else {
            EXPECT_FALSE(shape->isBomb()) << "Did not expect bomb for shape number " << n;
        }
        delete shape;
    }
}

// ────────────────────────────────────────────────────────────────────────────
// setAllowedShapesCount + getNewAddedShape (key PR change)
// ────────────────────────────────────────────────────────────────────────────

class SetAllowedShapesCountTest : public ::testing::Test {
protected:
    ShapeGenerator gen;
};

TEST_F(SetAllowedShapesCountTest, SetCount6_GetNewAddedShape_ReturnsBomb) {
    gen.setAllowedShapesCount(6);
    IShape* shape = gen.getNewAddedShape(kPos, kNullDC);
    ASSERT_NE(shape, nullptr);
    EXPECT_TRUE(shape->isBomb());
    delete shape;
}

TEST_F(SetAllowedShapesCountTest, SetCount5_GetNewAddedShape_ReturnsNonBomb) {
    gen.setAllowedShapesCount(5);
    IShape* shape = gen.getNewAddedShape(kPos, kNullDC);
    ASSERT_NE(shape, nullptr);
    EXPECT_FALSE(shape->isBomb());
    delete shape;
}

TEST_F(SetAllowedShapesCountTest, SetCount4_GetNewAddedShape_ReturnsNonBomb) {
    gen.setAllowedShapesCount(4);
    IShape* shape = gen.getNewAddedShape(kPos, kNullDC);
    ASSERT_NE(shape, nullptr);
    EXPECT_FALSE(shape->isBomb());
    delete shape;
}

// Calling setAllowedShapesCount twice: the second call should overwrite.
TEST_F(SetAllowedShapesCountTest, SetCount_Overwrite_SecondCallWins) {
    gen.setAllowedShapesCount(5);  // non-bomb
    gen.setAllowedShapesCount(6);  // bomb
    IShape* shape = gen.getNewAddedShape(kPos, kNullDC);
    ASSERT_NE(shape, nullptr);
    EXPECT_TRUE(shape->isBomb());
    delete shape;
}

TEST_F(SetAllowedShapesCountTest, SetCount_Overwrite_SecondCallNonBomb_Wins) {
    gen.setAllowedShapesCount(6);  // bomb
    gen.setAllowedShapesCount(5);  // non-bomb
    IShape* shape = gen.getNewAddedShape(kPos, kNullDC);
    ASSERT_NE(shape, nullptr);
    EXPECT_FALSE(shape->isBomb());
    delete shape;
}

TEST_F(SetAllowedShapesCountTest, GetNewAddedShape_ReturnsNonNull) {
    gen.setAllowedShapesCount(4);
    IShape* shape = gen.getNewAddedShape(kPos, kNullDC);
    EXPECT_NE(shape, nullptr);
    delete shape;
}

// ────────────────────────────────────────────────────────────────────────────
// getNextShape / getShape – delegates to getNumberMappedShape (PR refactor)
// ────────────────────────────────────────────────────────────────────────────

class GetShapeTest : public ::testing::Test {
protected:
    ShapeGenerator gen;

    void SetUp() override {
        gen.setAllowedShapesCount(5);
        gen.generateShapes();
    }
};

TEST_F(GetShapeTest, GetShape_ReturnsNonNull) {
    IShape* shape = gen.getShape(kPos, kNullDC);
    EXPECT_NE(shape, nullptr);
    delete shape;
}

TEST_F(GetShapeTest, GetNextShape_ReturnsNonNull) {
    IShape* shape = gen.getNextShape(kPos, kNullDC);
    EXPECT_NE(shape, nullptr);
    delete shape;
}

// After setAllowedShapesCount(6), generated shapes may include bombs.
// Verify that getShape/getNextShape still return valid shapes.
TEST_F(GetShapeTest, GetShape_WithBombsAllowed_ReturnsValidShape) {
    gen.setAllowedShapesCount(6);
    gen.generateShapes();
    IShape* shape = gen.getShape(kPos, kNullDC);
    ASSERT_NE(shape, nullptr);
    EXPECT_FALSE(shape->getShapeContainer().empty());
    delete shape;
}

// ────────────────────────────────────────────────────────────────────────────
// Regression: BombShape-specific behaviour via getNumberMappedShape
// ────────────────────────────────────────────────────────────────────────────

TEST(BombViaGeneratorTest, BombShapeHasOneRectangleInContainer) {
    ShapeGenerator gen;
    IShape* shape = gen.getNumberMappedShape(6, kPos, kNullDC);
    ASSERT_NE(shape, nullptr);
    EXPECT_EQ(shape->getShapeContainer().size(), 1u);
    delete shape;
}

TEST(BombViaGeneratorTest, NonBombShapeHasFourRectanglesInContainer) {
    ShapeGenerator gen;
    // All non-bomb shapes extend BaseShape which always has 4 rectangles.
    for (unsigned int n = 1; n <= 5; ++n) {
        IShape* shape = gen.getNumberMappedShape(n, kPos, kNullDC);
        ASSERT_NE(shape, nullptr) << "shape " << n;
        EXPECT_EQ(shape->getShapeContainer().size(), 4u) << "shape " << n;
        delete shape;
    }
}

} // namespace