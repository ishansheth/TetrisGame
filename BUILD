load("@rules_cc//cc:defs.bzl","cc_binary")

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
        'TEXTURE_FILE_PATH="/TetrisGame/resources/textures/brick_texture.png"',
        'BOMB_IMAGE="/TetrisGame/resources/textures/bomb_texture.png"',
        'SOUND_FOLDER_PATH="/TetrisGame/resources/sounds/"',
        'META_DATA_FILE_PATH="/TetrisGame/resources/files/"',
        'WINDOW_WIDTH=700',
        'WINDOW_HEIGHT=600'        
    ],
    copts = ["-std=c++17"]

)
