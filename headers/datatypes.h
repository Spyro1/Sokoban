#ifndef SOKOBAN2_DATATYPES_H
#define SOKOBAN2_DATATYPES_H
#include <stdbool.h>
#include "../libraries/econio.h"

// Makrók
#define nameLenght 20 // A játékos nevének maximális hossza
#define maxLineLenght 400
#define maxFileNameLenght 200

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
Point AddPoints(Point a, Point b);
/**
 * Két pont koordinátáinak különbségét kiszámoló függvény
 * @param a Egyik koordináta
 * @param b Másik koordniáta
 * @return Koordináták különbsége
 */
Point SubPoints(Point a, Point b);
/**
 * Két pontot hasonlít össze, hogy egyenlőek-e
 * @param a Egyik koordináta
 * @param b Másik koordináta
 * @return Egyenlőek e a paraméterként kapott koordináták
 */
bool EqualToPoint(Point a, Point b);

/* Letörli a cím alatt lévő területet a képernyőről */
void ClearScrBellow();
/**
 * Letörli a képernyőt megadott koordinátákon belül a kapott bgColor színnel.
 * @param x1 Bal felső sarok x koordinátája
 * @param y1 Bal felső sarok y koordinátája
 * @param x2 Jobb alsó sarok x koordinátája
 * @param y2 Jobb alsó sarok y koordinátája
 * @param bgColor Törlendő terület háttérszíne
 */
void ClearScreenSection(int x1, int y1, int x2, int y2, EconioColor bgColor);
/**
 * Kiírja a képernyőre a kapott szöveget a megadott kezdő koordinátákra
 * @param str Kiírandó szöveg
 * @param x x koordináta a képernyőn
 * @param y y koordniáta a képernyőn
 */
void print(char const str[], int x, int y);
/**
 * Kiírja a képernyőre a kapott szöveget a megadott kezdő koordinátákra a megadott betűszínnel
 * @param str Kiírandó szöveg
 * @param x x koordináta a képernyőn
 * @param y y koordniáta a képernyőn
 * @param foreColor A szöveg színe
 */
void printfc(char const str[], int x, int y, EconioColor foreColor);
/**
 * Kiírja a képernyőre a kapott szöveget a megadott kezdő koordinátákra a megadott betűszínnel és háttérszínnel
 * @param str Kiírandó szöveg
 * @param x x koordináta a képernyőn
 * @param y y koordniáta a képernyőn
 * @param foreColor A szöveg színe
 * @param bgColor A szöveg háttérszíne
 */
void printfbc(char const str[], int x, int y, EconioColor foreColor, EconioColor bgColor);

int stringlenght(const char *s);
int stringlenghtMax(const char *s, int max);
bool isBlankString(const char* str);
//char *cutAndFixString(const char *str);

#endif //SOKOBAN2_DATATYPES_H
