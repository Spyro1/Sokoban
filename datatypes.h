#ifndef SOKOBAN2_DATATYPES_H
#define SOKOBAN2_DATATYPES_H
#include <stdbool.h>

#define nameLenght 21
#define maxReadLineLenght 400 // 20 karakter név, 3karakter szint szám,
#define maxFileNameLenght 200
// == Adatstruktúrák ==

typedef enum celltype { null, EMPTY, WALL, TARGET, PLAYER, PLAYERONTARGET, BOX, BOXONTARGET  } CellType;

typedef struct position{
    int x, y;
    struct Point *next;
} Point;
typedef struct size{
    int width, height;
} Size;
typedef struct celldata{
    enum celltype cell;
    int count;
} CellData;

typedef struct player {
    char name[nameLenght];
    int numOfCompletedLevels;
    int *levelMoves;
    struct Player *next;//, *back;
} Player;

// Functions

Point AddPoints(Point a, Point b);

Point SubPoints(Point a, Point b);

bool EqualToPoint(Point a, Point b);

#endif //SOKOBAN2_DATATYPES_H
