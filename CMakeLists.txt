cmake_minimum_required(VERSION 3.26)
project(sokoban2 C)

set(CMAKE_C_STANDARD 11)

add_executable(sokoban2 main.c
        econio.c
        econio.h
        datatypes.c
        datatypes.h
        game.c
        main.h
        game.c
        game.h
        debugmalloc.h
        player.c
        player.h
        level.c
        level.h
)
