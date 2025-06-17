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
            "include/StageManager.h",
            "include/Util.h",
            "include/DisplayContainer.h",
            "include/GameConstants.h",
            "include/FontContainer.h",
            "include/LShape.h",
            "include/SqShape.h",
            "include/SShape.h",
            "include/StShape.h",
            "include/IShape.h",
            "include/BombShape.h",
            "include/ShapeGenerator.h",
            "include/TShape.h"],
    visibility = ["//visibility:public"],
    linkopts = [
        "-lsfml-graphics",
        "-lsfml-window",
        "-lsfml-system",
        "-lpthread"
    ],
    defines = [
        'FONT_FILE_PATH="/TetrisGame/movistar-text/Movistar Text Regular.ttf"',
        'SHAPE_TEXTURE_FILE="/TetrisGame/textures/brick_texture.png"',
        'WINDOW_WIDTH=700',
        'WINDOW_HEIGHT=600'        
    ],
    copts = ["-std=c++17"]

)
