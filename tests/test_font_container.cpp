// Tests for FontContainer changes introduced in this PR:
//   - 4 new GameFontStrings enum values:
//       GAME_KEYS_INSTRUCTION, GAME_NEW_SHAPE_ADDED_INFO,
//       BOMB_SHAPE_INSTRUCTION, FLOOR_RAISE_INSTRUCTION
//   - static_assert in FontContainer.cpp updated to reference FLOOR_RAISE_INSTRUCTION
//   - Both fontKeyToStrings and stringToLocation maps extended with the new keys
//
// These tests exercise the maps and enum directly without instantiating
// FontContainer (which would require a font file from disk).

#include <gtest/gtest.h>
#include "FontContainer.h"

namespace {

// ────────────────────────────────────────────────────────────────────────────
// GameFontStrings enum ordering / contiguity
// ────────────────────────────────────────────────────────────────────────────

TEST(GameFontStringsEnum, NEXT_SHAPE_LABEL_IsZero) {
    EXPECT_EQ(static_cast<int>(GameFontStrings::NEXT_SHAPE_LABEL), 0);
}

TEST(GameFontStringsEnum, MAXIMUM_NUMBER_OF_FONTS_FollowsFLOOR_RAISE_INSTRUCTION) {
    // This mirrors the static_assert added/updated in FontContainer.cpp.
    EXPECT_EQ(static_cast<int>(GameFontStrings::MAXIMUM_NUMBER_OF_FONTS),
              static_cast<int>(GameFontStrings::FLOOR_RAISE_INSTRUCTION) + 1);
}

// Make sure the newly added entries are between the existing ones and MAXIMUM.
TEST(GameFontStringsEnum, NewEntriesAreBeforeMAXIMUM) {
    EXPECT_LT(static_cast<int>(GameFontStrings::GAME_KEYS_INSTRUCTION),
              static_cast<int>(GameFontStrings::MAXIMUM_NUMBER_OF_FONTS));
    EXPECT_LT(static_cast<int>(GameFontStrings::GAME_NEW_SHAPE_ADDED_INFO),
              static_cast<int>(GameFontStrings::MAXIMUM_NUMBER_OF_FONTS));
    EXPECT_LT(static_cast<int>(GameFontStrings::BOMB_SHAPE_INSTRUCTION),
              static_cast<int>(GameFontStrings::MAXIMUM_NUMBER_OF_FONTS));
    EXPECT_LT(static_cast<int>(GameFontStrings::FLOOR_RAISE_INSTRUCTION),
              static_cast<int>(GameFontStrings::MAXIMUM_NUMBER_OF_FONTS));
}

// New entries must come after WINDOW_CLOSE_HIGHSCORE_SAVED (the last pre-PR entry).
TEST(GameFontStringsEnum, NewEntriesAreAfterWINDOW_CLOSE_HIGHSCORE_SAVED) {
    EXPECT_GT(static_cast<int>(GameFontStrings::GAME_KEYS_INSTRUCTION),
              static_cast<int>(GameFontStrings::WINDOW_CLOSE_HIGHSCORE_SAVED));
    EXPECT_GT(static_cast<int>(GameFontStrings::GAME_NEW_SHAPE_ADDED_INFO),
              static_cast<int>(GameFontStrings::WINDOW_CLOSE_HIGHSCORE_SAVED));
    EXPECT_GT(static_cast<int>(GameFontStrings::BOMB_SHAPE_INSTRUCTION),
              static_cast<int>(GameFontStrings::WINDOW_CLOSE_HIGHSCORE_SAVED));
    EXPECT_GT(static_cast<int>(GameFontStrings::FLOOR_RAISE_INSTRUCTION),
              static_cast<int>(GameFontStrings::WINDOW_CLOSE_HIGHSCORE_SAVED));
}

TEST(GameFontStringsEnum, FLOOR_RAISE_INSTRUCTION_IsDirectlyBeforeMAXIMUM) {
    EXPECT_EQ(static_cast<int>(GameFontStrings::FLOOR_RAISE_INSTRUCTION) + 1,
              static_cast<int>(GameFontStrings::MAXIMUM_NUMBER_OF_FONTS));
}

// ────────────────────────────────────────────────────────────────────────────
// fontKeyToStrings map – new entries
// ────────────────────────────────────────────────────────────────────────────

TEST(FontKeyToStringsMap, ContainsGAME_KEYS_INSTRUCTION) {
    EXPECT_NE(fontKeyToStrings.find(GameFontStrings::GAME_KEYS_INSTRUCTION),
              fontKeyToStrings.end());
}

TEST(FontKeyToStringsMap, ContainsGAME_NEW_SHAPE_ADDED_INFO) {
    EXPECT_NE(fontKeyToStrings.find(GameFontStrings::GAME_NEW_SHAPE_ADDED_INFO),
              fontKeyToStrings.end());
}

TEST(FontKeyToStringsMap, ContainsBOMB_SHAPE_INSTRUCTION) {
    EXPECT_NE(fontKeyToStrings.find(GameFontStrings::BOMB_SHAPE_INSTRUCTION),
              fontKeyToStrings.end());
}

TEST(FontKeyToStringsMap, ContainsFLOOR_RAISE_INSTRUCTION) {
    EXPECT_NE(fontKeyToStrings.find(GameFontStrings::FLOOR_RAISE_INSTRUCTION),
              fontKeyToStrings.end());
}

TEST(FontKeyToStringsMap, GAME_KEYS_INSTRUCTION_StringIsNonEmpty) {
    EXPECT_FALSE(fontKeyToStrings.at(GameFontStrings::GAME_KEYS_INSTRUCTION).empty());
}

TEST(FontKeyToStringsMap, GAME_NEW_SHAPE_ADDED_INFO_StringIsNonEmpty) {
    EXPECT_FALSE(fontKeyToStrings.at(GameFontStrings::GAME_NEW_SHAPE_ADDED_INFO).empty());
}

TEST(FontKeyToStringsMap, BOMB_SHAPE_INSTRUCTION_StringIsNonEmpty) {
    EXPECT_FALSE(fontKeyToStrings.at(GameFontStrings::BOMB_SHAPE_INSTRUCTION).empty());
}

TEST(FontKeyToStringsMap, FLOOR_RAISE_INSTRUCTION_StringIsNonEmpty) {
    EXPECT_FALSE(fontKeyToStrings.at(GameFontStrings::FLOOR_RAISE_INSTRUCTION).empty());
}

// Verify the exact string values specified in the PR.
TEST(FontKeyToStringsMap, GAME_KEYS_INSTRUCTION_HasExpectedContent) {
    const auto& s = fontKeyToStrings.at(GameFontStrings::GAME_KEYS_INSTRUCTION);
    // Must mention ENTER key (Press [ENTER]) as that's what showInstructionScreen() waits for.
    EXPECT_NE(s.find("[ENTER]"), std::string::npos);
}

TEST(FontKeyToStringsMap, GAME_NEW_SHAPE_ADDED_INFO_HasExpectedContent) {
    const auto& s = fontKeyToStrings.at(GameFontStrings::GAME_NEW_SHAPE_ADDED_INFO);
    EXPECT_EQ(s, "New Shape available:");
}

TEST(FontKeyToStringsMap, BOMB_SHAPE_INSTRUCTION_MentionsBomb) {
    const auto& s = fontKeyToStrings.at(GameFontStrings::BOMB_SHAPE_INSTRUCTION);
    EXPECT_NE(s.find("Bomb"), std::string::npos);
}

TEST(FontKeyToStringsMap, FLOOR_RAISE_INSTRUCTION_HasExpectedContent) {
    const auto& s = fontKeyToStrings.at(GameFontStrings::FLOOR_RAISE_INSTRUCTION);
    EXPECT_EQ(s, "Floor is raising, be quick!!");
}

// ────────────────────────────────────────────────────────────────────────────
// stringToLocation map – new entries
// ────────────────────────────────────────────────────────────────────────────

TEST(StringToLocationMap, ContainsGAME_KEYS_INSTRUCTION) {
    EXPECT_NE(stringToLocation.find(GameFontStrings::GAME_KEYS_INSTRUCTION),
              stringToLocation.end());
}

TEST(StringToLocationMap, ContainsGAME_NEW_SHAPE_ADDED_INFO) {
    EXPECT_NE(stringToLocation.find(GameFontStrings::GAME_NEW_SHAPE_ADDED_INFO),
              stringToLocation.end());
}

TEST(StringToLocationMap, ContainsBOMB_SHAPE_INSTRUCTION) {
    EXPECT_NE(stringToLocation.find(GameFontStrings::BOMB_SHAPE_INSTRUCTION),
              stringToLocation.end());
}

TEST(StringToLocationMap, ContainsFLOOR_RAISE_INSTRUCTION) {
    EXPECT_NE(stringToLocation.find(GameFontStrings::FLOOR_RAISE_INSTRUCTION),
              stringToLocation.end());
}

// All four new instruction strings share position {250, 240} as per the PR.
TEST(StringToLocationMap, GAME_KEYS_INSTRUCTION_PositionIs250_240) {
    const auto& pos = stringToLocation.at(GameFontStrings::GAME_KEYS_INSTRUCTION);
    EXPECT_FLOAT_EQ(pos.x, 250.f);
    EXPECT_FLOAT_EQ(pos.y, 240.f);
}

TEST(StringToLocationMap, GAME_NEW_SHAPE_ADDED_INFO_PositionIs250_240) {
    const auto& pos = stringToLocation.at(GameFontStrings::GAME_NEW_SHAPE_ADDED_INFO);
    EXPECT_FLOAT_EQ(pos.x, 250.f);
    EXPECT_FLOAT_EQ(pos.y, 240.f);
}

TEST(StringToLocationMap, BOMB_SHAPE_INSTRUCTION_PositionIs250_240) {
    const auto& pos = stringToLocation.at(GameFontStrings::BOMB_SHAPE_INSTRUCTION);
    EXPECT_FLOAT_EQ(pos.x, 250.f);
    EXPECT_FLOAT_EQ(pos.y, 240.f);
}

TEST(StringToLocationMap, FLOOR_RAISE_INSTRUCTION_PositionIs250_240) {
    const auto& pos = stringToLocation.at(GameFontStrings::FLOOR_RAISE_INSTRUCTION);
    EXPECT_FLOAT_EQ(pos.x, 250.f);
    EXPECT_FLOAT_EQ(pos.y, 240.f);
}

// ────────────────────────────────────────────────────────────────────────────
// Map completeness – every GameFontStrings value (except MAXIMUM) must appear
// in both maps so that FontContainer::addFont() never throws std::out_of_range.
// ────────────────────────────────────────────────────────────────────────────

TEST(MapCompleteness, fontKeyToStrings_ContainsAllEnumValues) {
    for (int i = 0; i < static_cast<int>(GameFontStrings::MAXIMUM_NUMBER_OF_FONTS); ++i) {
        auto key = static_cast<GameFontStrings>(i);
        EXPECT_NE(fontKeyToStrings.find(key), fontKeyToStrings.end())
            << "Missing fontKeyToStrings entry for GameFontStrings value " << i;
    }
}

TEST(MapCompleteness, stringToLocation_ContainsAllEnumValues) {
    for (int i = 0; i < static_cast<int>(GameFontStrings::MAXIMUM_NUMBER_OF_FONTS); ++i) {
        auto key = static_cast<GameFontStrings>(i);
        EXPECT_NE(stringToLocation.find(key), stringToLocation.end())
            << "Missing stringToLocation entry for GameFontStrings value " << i;
    }
}

} // namespace