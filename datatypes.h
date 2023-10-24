#ifndef SOKOBAN2_DATATYPES_H
#define SOKOBAN2_DATATYPES_H

#define nameLenght 21
#define maxReadLineLenght 200
#define maxFileNameLenght 100
// == Adatstruktúrák ==

typedef enum celltype { null, EMPTY, WALL, TARGET, PLAYER, PLAYERONTARGET, BOX, BOXONTARGET  } CellType;

typedef struct position{
    int x, y;
} Point;
typedef struct size{
    int width, height;
} Size;

typedef struct cell{
    Point pos;
    CellType type;
} Cell;

typedef struct player {
    char name[nameLenght];
    int completedLevels,
            totalMoves,
            averageMoves;
    struct Player *next;//, *back;
} Player;


#endif //SOKOBAN2_DATATYPES_H
