#ifndef SOKOBAN2_DATATYPES_H
#define SOKOBAN2_DATATYPES_H
#include <stdbool.h>
#include "../libraries/econio.h"

// Makrók
#define nameLenght 20 // A játékos nevének maximális hossza
#define maxLineLenght 400 // A fájlból beolvasható sor maximális hossza
#define maxFileNameLenght 200 // A féjlnév maximális hossza

#define activeForeColor COL_LIGHTCYAN
#define activeBgColor COL_RESET
#define baseForeColor COL_LIGHTBLUE
// == Adatstruktúrák ==

/* A pálya egyes mezőinek lehetséges értékei.\Értékek null, EMPTY, WALL, TARGET, PLAYER, PLAYERONTARGET, BOX, BOXONTARGET */
typedef enum celltype { null, EMPTY, WALL, TARGET, PLAYER, PLAYERONTARGET, BOX, BOXONTARGET } CellType;

/* A menü állapotait leíró enum */
typedef enum State { mainMenu, newPlayer, chosePlayer, rankList, exitApp, deletePlayer, editPlayer } State;

/* Egy koordinátát eltároló struktúra, mely láncolt listába fűzhető (x,y,*next) */
typedef struct position{
    int x, y;
    struct Point *next;
} Point;

/* A pálya méretét eltároló struktúra (hossz, szélesség) */
typedef struct size{
    int width, height;
} Size;

/* A játékos egy lépését tároló struktúra, mely láncolt listába fűzhető (from, to, boxPushed, *next)*/
typedef struct move{
    Point from;
    Point to;
    bool boxPushed;
    struct Move *next;
} Move;

/* A játékos egy szinten megtett lépéseinek számát tároló struktúra, mely láncolt listába fűzhető (stepCount, *next) */
typedef struct statistic{
    int stepCount;
    struct Statistics *next;
} Statistics;

/* A játékos adatait eltároló struktúra, mely láncolt listába fűzhető
 * (name: neve,
 * numOfCompletedLevels: teljesített szintek száma,
 * levelStats: láncolt lista,
 * *next: következő elem)*/
typedef struct player {
    char name[nameLenght*2+1];
    int numOfCompletedLevels;
    struct Statistics *levelStats;
    struct Player *next;
} Player;


// Functions
/**
 * Két pont koordinátáit összeadó függvény
 * @param a Egyik koordináta
 * @param b Másik koordináta
 * @return Koordináták összege
 */
Point addPoints(Point a, Point b);
/**
 * Két pont koordinátáinak különbségét kiszámoló függvény
 * @param a Egyik koordináta
 * @param b Másik koordniáta
 * @return Koordináták különbsége
 */
Point subPoints(Point a, Point b);
/**
 * Két pontot hasonlít össze, hogy egyenlőek-e
 * @param a Egyik koordináta
 * @param b Másik koordináta
 * @return Egyenlőek e a paraméterként kapott koordináták
 */
bool comparePoints(Point a, Point b);


#endif //SOKOBAN2_DATATYPES_H
