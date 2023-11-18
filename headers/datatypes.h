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
typedef enum State { mainMenu, newPlayer, chosePlayer, rankList, exitApp, deletePlayer, editPlayer, game } State;

/* Egy koordinátát eltároló struktúra, mely láncolt listába fűzhető (x,y,*next) */
typedef struct position{
    int x, y; // Koordinátál
    struct Point *next; // A következő pontra mutató pointer a láncolt listában
} Point;

/* A pálya méretét eltároló struktúra */
typedef struct size{
    int width; // A pálya szélessége
    int height; // A pálya magassága
} Size;

/* A játékos egy lépését tároló struktúra, mely láncolt listába fűzhető */
typedef struct move{
    Point from; // A játékos által elhagyott mező koordinátája
    Point to; // A játékos által meglépett mező koordinátája
    bool boxPushed; // Logikai, eltolt-e a játékos a lépés során dobozt
    struct Move *next; // a lépéseket tároló láncolt listában a következő elemre mutató pointer
} Move;

/* A játékos egy szinten megtett lépéseinek számát tároló struktúra, mely láncolt listába fűzhető */
typedef struct statistic{
    int stepCount; // Egy szinten a játékos által megtett lépések száma
    struct Statistics *next; // A statisztika láncolt listában a következő elemre mutató pointer
} Statistics;

/* A játékos adatait eltároló struktúra, mely láncolt listába fűzhető */
typedef struct player {
    char name[nameLenght*2+1]; // Játékosnév: maximum nameLenght hosszú (*2+1 az ékezetes karakterek és a \0 miatt)
    int numOfCompletedLevels; // Teljesített szintek száma
    struct Statistics *levelStats; // A teljesített szinteken megtett lépések száma láncolt listában
    struct Player *next; // A játékos láncolt listában a következő elemre mutató pointer
} Player;

// Függvények

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