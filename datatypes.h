#ifndef SOKOBAN2_DATATYPES_H
#define SOKOBAN2_DATATYPES_H

/* == Adatstruktúrák == */

typedef enum celltype { PLAYER, AIR, WALL, BOX, TARGET, EMPTY } CellType;

typedef struct position{
    int x, y;
} Point;

typedef struct cell{
    Point pos;
    CellType type;
} Cell;

typedef struct player {
    char name[50];
    int completedLevels,
            totalMoves,
            averageMoves;
    struct Player *next, *back;
} Player;


#endif //SOKOBAN2_DATATYPES_H
