cmake_minimum_required(VERSION 3.26)
project(sokoban2 C)

set(CMAKE_C_STANDARD 11)

add_executable(sokoban2 main.c
        econio.c
        econio.h
        game.c
        main.h
        datatypes.c
        datatypes.h
        game.h
)
