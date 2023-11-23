#ifndef SOKOBAN2_DATATYPES_H
#define SOKOBAN2_DATATYPES_H
#include <stdbool.h>
#include "../libraries/econio.h"

// Makrók
#define nameLenght 20           // A játékos nevének maximális hossza
#define maxLineLenght 400       // A fájlból beolvasható sor maximális hossza
#define maxFileNameLenght 200   // A féjlnév maximális hossza

#define activeForeColor COL_LIGHTCYAN   // Kiválasztás betűszine
#define activeBgColor COL_RESET         // Kiválasztás háttérszíne
#define baseForeColor COL_LIGHTBLUE     // Alap betűszín
// == Adatstruktúrák ==

/** A pálya egyes mezőinek lehetséges értékei */
typedef enum celltype {
    null,
    EMPTY,
    WALL,
    TARGET,
    PLAYER,
    PLAYERONTARGET,
    BOX,
    BOXONTARGET
} CellType;

/** A menü lehetséges állapotértékei */
typedef enum State {
    mainMenu,
    newPlayer,
    chosePlayer,
    rankList,
    exitApp,
    deletePlayer,
    editPlayer,
    game,
    exitGame,
    winGame
} State;

/** Egy koordinátát eltároló struktúra, mely láncolt listába fűzhető */
typedef struct point{
    int x, y;           // Koordináták
    //struct Point *next; // A következő pontra mutató pointer a láncolt listában
} Point;

/** A pálya méretét eltároló struktúra */
typedef struct size{
    int width;  // A pálya szélessége
    int height; // A pálya magassága
} Size;

/** A játékos egy lépését tároló struktúra, mely láncolt listába fűzhető */
typedef struct move{
    Point from;         // A játékos által elhagyott mező koordinátája
    Point to;           // A játékos által meglépett mező koordinátája
    bool boxPushed;     // Logikai, eltolt-e a játékos a lépés során dobozt
    struct Move *next;  // A lépéseket tároló láncolt listában a következő elemre mutató pointer
} Move;

/** A játékos egy szinten megtett lépéseinek számát tároló struktúra, mely láncolt listába fűzhető */
typedef struct statistic{
    int stepCount;           // Egy szinten a játékos által megtett lépések száma
    struct Statistics *next; // A statisztika láncolt listában a következő elemre mutató pointer
} Statistics;

/** A játékos adatait eltároló struktúra, mely láncolt listába fűzhető */
typedef struct player {
    char name[nameLenght*2+1];      // Játékosnév: maximum nameLenght hosszú (*2+1 az ékezetes karakterek és a \0 miatt)
    int numOfCompletedLevels;       // Teljesített szintek száma
    struct Statistics *levelStats;  // A teljesített szinteken megtett lépések száma láncolt listában
    struct Player *next;            // A játékos láncolt listában a következő elemre mutató pointer
} Player;

#endif //SOKOBAN2_DATATYPES_H