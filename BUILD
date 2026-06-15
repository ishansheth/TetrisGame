load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_test")

cc_binary(
    name = "tetris",
    includes = ["include"],
    srcs = ["main.cpp",
            "src/FontContainer.cpp",
            "src/Util.cpp",
            "src/DisplayContainer.cpp",
            "src/ShapeGenerator.cpp",
            "src/StageManager.cpp",
            "src/ParticleSystem.cpp",
            "src/MetaFileHandler.cpp",
            "include/StageManager.h",
            "include/Util.h",
            "include/ParticleSystem.h",
            "include/DisplayContainer.h",
            "include/GameConstants.h",
            "include/FontContainer.h",
            "include/LShape.h",
            "include/BaseShape.h",
            "include/SqShape.h",
            "include/SShape.h",
            "include/StShape.h",
            "include/IShape.h",
            "include/BombShape.h",
            "include/SingleSquareShape.h",
            "include/ShapeGenerator.h",
            "include/MetaFileHandler.h",
            "include/TShape.h"],
    visibility = ["//visibility:public"],
    linkopts = [
        "-lsfml-graphics",
        "-lsfml-window",
        "-lsfml-system",
        "-lsfml-audio",
        "-lpthread"
    ],
    defines = [
        'FONTS_FILE_PATH="/TetrisGame/resources/fonts/Movistar Text Regular.ttf"',
        'BRICK_TEXTURE_FILE_PATH="/TetrisGame/resources/textures/brick_texture.png"',
        'TETRIS_POSTER_TEXTURE_FILE_PATH="/TetrisGame/resources/textures/tetris_poster.png"',
        'BOMB_IMAGE="/TetrisGame/resources/textures/bomb_texture.png"',
        'SOUND_FOLDER_PATH="/TetrisGame/resources/sounds/"',
        'META_DATA_FILE_PATH="/TetrisGame/resources/files/"',
        'WINDOW_WIDTH=700',
        'WINDOW_HEIGHT=600'        
    ],
    copts = ["-std=c++17"]

)

# Common SFML link options shared by all test targets.
_SFML_LINKOPTS = [
    "-lsfml-graphics",
    "-lsfml-window",
    "-lsfml-system",
    "-lsfml-audio",
    "-lpthread",
]

# Preprocessor defines required to compile the game headers in test mode.
# Resource file paths can point anywhere; SFML fails silently if files are absent.
_TEST_DEFINES = [
    'BRICK_TEXTURE_FILE_PATH="/nonexistent/brick_texture.png"',
    'TETRIS_POSTER_TEXTURE_FILE_PATH="/nonexistent/tetris_poster.png"',
    'BOMB_IMAGE="/nonexistent/bomb_texture.png"',
    'FONTS_FILE_PATH="/nonexistent/font.ttf"',
    'SOUND_FOLDER_PATH="/nonexistent/sounds/"',
    'META_DATA_FILE_PATH="/nonexistent/files/"',
    'WINDOW_WIDTH=700',
    'WINDOW_HEIGHT=600',
]

# Headers shared by shape-related tests.
_SHAPE_HEADERS = [
    "include/IShape.h",
    "include/BaseShape.h",
    "include/BombShape.h",
    "include/LShape.h",
    "include/SShape.h",
    "include/SqShape.h",
    "include/StShape.h",
    "include/TShape.h",
    "include/SingleSquareShape.h",
    "include/ShapeGenerator.h",
    "include/DisplayContainer.h",
    "include/FontContainer.h",
    "include/GameConstants.h",
    "include/ParticleSystem.h",
    "include/StageManager.h",
    "include/MetaFileHandler.h",
    "include/Util.h",
]

# ---------------------------------------------------------------------------
# test_shape_generator
#
# Tests for ShapeGenerator::getNumberMappedShape(), getNewAddedShape(), and
# setAllowedShapesCount() introduced in this PR.
# Run with: bazel test //:test_shape_generator
# ---------------------------------------------------------------------------
cc_test(
    name = "test_shape_generator",
    srcs = [
        "tests/test_shape_generator.cpp",
        # ShapeGenerator implementation (contains getNumberMappedShape etc.)
        "src/ShapeGenerator.cpp",
        # Util implementation (provides isWithinDrawWindow used by shape methods)
        "src/Util.cpp",
        # DisplayContainer.cpp is intentionally excluded: SHAPE_DOWN_FALL_SPEED_Y
        # is defined in the test file itself to avoid linking the full container
        # (which would pull in sound, particles, and high-score subsystems).
    ] + _SHAPE_HEADERS,
    includes = ["include"],
    deps = ["@com_google_googletest//:gtest_main"],
    linkopts = _SFML_LINKOPTS,
    defines = _TEST_DEFINES,
    copts = ["-std=c++17"],
)

# ---------------------------------------------------------------------------
# test_font_container
#
# Tests for the new GameFontStrings enum values and the corresponding entries
# in fontKeyToStrings / stringToLocation (FontContainer.h changes in this PR).
# No FontContainer instantiation – pure map/enum tests, no SFML window needed.
# Run with: bazel test //:test_font_container
# ---------------------------------------------------------------------------
cc_test(
    name = "test_font_container",
    srcs = [
        "tests/test_font_container.cpp",
        "include/FontContainer.h",
        "include/GameConstants.h",
    ],
    includes = ["include"],
    deps = ["@com_google_googletest//:gtest_main"],
    linkopts = _SFML_LINKOPTS,
    defines = _TEST_DEFINES,
    copts = ["-std=c++17"],
)

# ---------------------------------------------------------------------------
# test_bomb_shape
#
# Tests for BombShape::moveShape() always returning true after the PR removed
# the isWithinDrawWindow / isIntersecting guards.
# Run with: bazel test //:test_bomb_shape
# ---------------------------------------------------------------------------
cc_test(
    name = "test_bomb_shape",
    srcs = [
        "tests/test_bomb_shape.cpp",
        # Util implementation (isWithinDrawWindow used by BombShape::handleKey and
        # BaseShape::moveShape, even though the PR removed it from BombShape::moveShape).
        "src/Util.cpp",
        # SHAPE_DOWN_FALL_SPEED_Y is defined in the test file itself.
    ] + _SHAPE_HEADERS,
    includes = ["include"],
    deps = ["@com_google_googletest//:gtest_main"],
    linkopts = _SFML_LINKOPTS,
    defines = _TEST_DEFINES,
    copts = ["-std=c++17"],
)