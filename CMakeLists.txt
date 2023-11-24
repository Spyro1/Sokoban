cmake_minimum_required(VERSION 3.26)
project(sokoban2 C)

set(CMAKE_C_STANDARD 11)

add_executable(sokoban2 src/main.c
        libraries/econio.c
        libraries/econio.h
        src/datatypes.c
        headers/datatypes.h
        src/game.c
        src/game.c
        headers/game.h
        libraries/debugmalloc.h
        src/player.c
        headers/player.h
        src/level.c
        headers/level.h
        src/move.c
        headers/move.h
        src/statistics.c
        headers/statistics.h
        src/menu.c
        headers/menu.h
        src/lib.c
        headers/lib.h
)
#target_compile_options(sokoban2 PRIVATE -Wall -Werror)