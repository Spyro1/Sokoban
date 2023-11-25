#ifndef GITIGNORE_GAME_H
#define GITIGNORE_GAME_H
#include "../libraries/econio.h"
#include "datatypes.h"
#include "player.h"


// == Makrók ==
// Karakterek
#define chrWall "█" // Fal
#define chrPlayer "☻" // Játékos
#define chrTarget "◻" // Célmező
#define chrBox "◼" // Doboz

// Karaktererszínek
#define clrWall COL_DARKGRAY
#define clrPlayer COL_CYAN
#define clrPlayerOnTarget COL_RED
#define clrTarget COL_LIGHTRED
#define clrBox COL_BROWN
#define clrBoxOnTarget COL_GREEN

// == JÁTÉKMENET ==
/**
 * Ezzel kell meghívni a játékot. Inicializálja a játékhoz szükséges elemeket.
 * @param player Player* Az aktuális játékos adatait tartalmazza. (Cím szerint)
 * @param levelList char[][] A pályák fájlneveit tartalmazó string tömb.
 * @return bool Igaz, ha a játékos teljesítette a szintet; Hamis, ha a játékos kilépett a játékból.
 */
bool game_Init(Player *player, char **levelList);
/**
 * Kiértékeli a felhasználó által lenyomott billentyőt, és megváltoztatja a játék vagy a játékosbábu helyzetét aszerint.
 * @param map CellType** A pályát leíró 2D-s dinamikus mátrix.
 * @param mapSize Size A pálya szélességét és magasságát leíró struktúra.
 * @param numOfMoves int* A megtett lépések száma a szinten. (Cím szeirnt)
 * @param player Player* Az aktuális játékos adatait tartalmazza. (Cím szerint)
 * @param playerPosition Point* A játékos kezdő koordinátája a pályán (map-en). (Cím szerint)
 * @param boxPositions Point* A dobozok koordinátáinak dinamikus tömbje. (Cím szerint)
 * @param movesListHead Move** A játékos lépéseit eltároló láncolt lista. (Cím szerint)
 * @return bool Igaz, ha újraindítja a játékos a szinten; Különben hamis
 */
static bool game_KeyPress(CellType **map, Size mapSize, int *numOfMoves, Player *player, Point *playerPosition, Point *boxPositions, Move **movesListHead);
/**
 * Ez a függvény indítja el és futtatja ciklikusan a játékot.
 * @param player Player* Az aktuális játékos adatait tartalmazza. (Cím szerint)
 * @param levelName char[] A betöltendő pálya fájlneve.
 * @return bool Igaz, ha a játékos teljesítette a szintet; Hamis, ha a játékos kilépett a játékból.
 */
static bool game_StartGame(Player *player, char levelName[]);
/**
 * Ellenőrzi, hogy a játékos teljesítette-e a szintet, vagyis, hogy minden doboz a helyére került-e.
 * @param map CellType** A pályát leíró 2D-s dinamikus mátrix.
 * @param mapSize Size A pálya szélességét és magasságát leíró struktúra.
 * @return bool Igaz, ha minden doboz a helyére került; Hamis, ha van egy doboz is, ami nincs a helyén.
 */
static bool game_CheckWin(CellType **map, Size mapSize);
 /**
 * A játékos elmozdulását tesztelő függvény a direction irányba.
 * Ha lehetséges a lépés, vagy ha doboz van a lépés irányában akkor a dobozt eltolja, és igazat ad vissza; Hamis, ha nem lehetséges a lépés.
 * @param map CellType*** A pályát leíró 2D-s dinamikus mátrix. (Cím szerint)
 * @param currentPosition Point* A játékos aktuális koordinátája a pályán (map-en). (Cím szerint)
 * @param boxPositions Point** A dobozok koordinátáinak dinamikus tömbje. (Cím szerint)
 * @param direction Point A játékos elmozdulásvektora.
 * @param movesListHead Move** A játékos lépéseit eltároló láncolt lista. (Cím szerint)
 * @return bool Igaz, ha el tud mozdulni a játékos az adott irányba; Hamis, ha nem lehetséges a lépés.
 */
static bool game_MovePlayer(CellType ***map, Point *currentPosition, Point **boxPositions, Point direction, Move **movesListHead);
/**
 * Visszavonja a játékos előző lépést. Egészen addig fut le sikeresen,
 * amíg a moveList-ben volt elem, azaz meglépett lépés.
 * @param map Celltype** A pályát leíró 2D-s dinamikus mátrix (Cím szerint)
 * @param currentPosition Point* A játékos aktuális koordinátája a pályán (map-en) (Cím szerint)
 * @param boxPositions Point** A dobozok koordinátáinak dinamikus tömbje (Cím szerint)
 * @param moveListHead Move** A játékos lépéseit eltároló láncolt lista (Cím szerint)
 * @return bool Igaz, ha sikeresen visszavonta a lépést; Hamis, ha nincs több visszavonható lépés
 */
static bool game_UndoMove(CellType ***map, Point *currentPosition, Point **boxPositions, Move **moveListHead);

// == BEOLVASÁS ==
/**
 * Beolvassa a kapott fájlnévben lévő pályát és eltárolja a map mátrixban
 * @param filename char[] A pálya fájlneve (Bemenet)
 * @param map CellType*** A pályát leíró 2D-s dinamikus mátrix (Cím szerint, kimenet)
 * @param mapSize Size* A pálya szélességét és magasságát leíró struktúra (Cím szerint, kimenet)
 * @param playerPosition Point* A játékos kezdő koordinátája a pályán (map-en) (Cím szerint, kimenet)
 * @param boxPositions Point** A dobozok koordinátáinak dinamikus tömbje (Cím szerint, kimenet)
 * @param boxCount int* A dobozok koordinátáit tároló dinamikus tömb elemszáma, dobozok száma a pályán (Cím szerint, kimenet)
 */
static void game_ReadXSBFile(char filename[], CellType ***map, Size *mapSize, Point *playerPosition, Point **boxPositions, int *boxCount);
/**
 * A beolvasott fájl egy karakterét értelmezi és átalakítja cellType értékké
 * @param character char char A beolvasott fájl egy karaktere
 * @return CellType A kapott karakter értelmezett CellType értékekké alakított értéke
 */
static CellType game_ConvertInputCharToCellType(char character);

// == KIIRATÁS ==
/**
 * Kiiratja a pályát a képernyőre színesen
 * @param map CellType** A pályát leíró 2D-s dinamikus mátrix
 * @param mapSize Size A pálya szélességét és magasságát leíró struktúra
 */
static void game_PrintStyledMap(CellType **map, Size mapSize);
/**
 * Egy kapott koordinátán lévő mezőt írja ki a képernyőre színesen és a megfelelő definiált karaterrel
 * @param map CellType** A pályát leíró 2D-s dinamikus mátrix
 * @param pos Point A kiirandó karakter koordinátája
 */
static void game_PrintPosition(CellType **map, Point pos);
/**
 * Kiírja a képernyőre a játéktér mellé az aktuális szintet és a lépések számát, vagy a tutorial pályánál a bevezető instrukciókat
 * @param mapSize Size A pálya szélességét és magasságát leíró struktúra
 * @param numOfSteps int A szinten megtett lépések száma
 * @param level int Az aktuális szint száma
 */
static void game_PrintStatsAndNav(Size mapSize, int numOfSteps, int level);

// == MEMÓRIAFOGLALÁS ÉS FELSZABADÍTÁS ==
/**
 * Memóriát foglal a pályát tároló 2D-s dinamikus mátrixnak (map-nek)
 * @param map CellType*** A pályát leíró 2D-s dinamikus mátrix (Cím szerint)
 * @param mapSize Size* A pálya szélességét és magasságát leíró struktúra
 */
static void game_AllocateMemoryToMap(CellType ***map, Size *mapSize);
/**
 * Memóriát foglal egy 1D-s dinamikus tömbnek
 * @param newArray Point** Az új 1D-s dinamikus tömb címe (Cím szerint)
 * @param lenght int A létrehozandó dinamikus tömb hossza, elemszáma
 */
static void game_AllocateDynamicArray(Point **newArray, int lenght);
/**
 * Felszabadítja a pályát tároló 2D-s dinamikus mátrix lefoglat memóriáját, ha volt lefoglalva
 * @param map Celltype*** A pályát leíró 2D-s dinamikus mátrix (Cím szerint)
 */
static void game_FreeAllocatedMemoryFromMap(CellType ***map);
/**
 * Felszabadítja egy 1D-s dinamikus tömbnek lefoglalt memóriáját, ha volt lefoglalva
 * @param dynamicArray Point** 1D-s dinamikus tömb címe (Cím szerint)
 */
static void game_FreeDynamicArray(Point **dynamicArray);

#endif //GITIGNORE_GAME_H
