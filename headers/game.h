#ifndef GITIGNORE_GAME_H
#define GITIGNORE_GAME_H
#include "../libraries/econio.h"
#include "datatypes.h"
#include "player.h"


/* == Konstansok == */
// Karakterek
#define chrWall "█"
#define chrPlayer "☻" //◯
#define chrTarget "◻" // "◻□◼
#define chrBox "◼" //▩◼⛝ //  ▩◼⛝ ◻□◼

// Színek
#define clrWall COL_DARKGRAY
#define clrPlayer COL_CYAN
#define clrPlayerOnTarget COL_RED
#define clrTarget COL_LIGHTRED
#define clrBox COL_BROWN
#define clrBoxOnTarget COL_GREEN

// == JÁTÉKMENET ==
/**
 * Ezzel kell meghívni a játékot, inicializálja a játékhoz szükséges dolgokat
 * @param player Az aktuális játékos adatait tartalmazza (cím szerint átadva)
 * @param levelList Dinamikus tömb, a pályák fájlneveit tartalmazza (level.c hozza létre)
 * @return Logikai: Igaz, ha a játékos teljesítette a szintet; Hamis, ha a játékos kilépett a játékból
 */
bool Init(Player *player, char **levelList);
/**
 * Ezzel indítható el a játék menete
 * @param player Az aktuális játékos adatait tartalmazza (cím szerint átadva)
 * @param levelName A betöltendő pálya fájlneve
 * @return Logikai: Igaz, ha a játékos teljesítette a szintet; Hamis, ha a játékos kilépett a játékból
 */
bool StartGame(Player *player, char levelName[]);
/**
 * Ellenőrzi, hogy a játékos teljesítette-e a szintet, vagyis, hogy minden doboz a helyére került-e
 * @param map A pályát leíró 2D-s dinamikus mátrix
 * @param mapSize A pálya szélességét és magasságát leíró struktúra
 * @return Logikai: Igaz, ha minden doboz a helyére került; Hamis, ha van egy doboz is, ami nincs a helyén
 */
bool CheckWin(CellType **map, Size mapSize);
/**
 * A játékos elmozdulását tesztelő függvény a direction irányba.
 * Ha lehetséges a lépés, akor igazat ad vissza, és ha doboz van a lépés irányában akkor a dobozt eltolja, és igazat ad vissza.
 * @param map A pályát leíró 2D-s dinamikus mátrix (cím szerint)
 * @param mapSize A pálya szélességét és magasságát leíró struktúra
 * @param currentPosition A játékos aktuális koordinátája a pályán (map-en) (cím szerint)
 * @param boxPositions A dobozok koordinátáinak dinamikus tömbje cím szerint átadva (cím szerint)
 * @param direction A játékos elmozdulásvektora
 * @param movesListHead A játékos lépéseit eltároló láncolt lista cím szerint átadva (cím szerint)
 * @return Logikai: IGAZ, ha el tud mozdulni a játékos az adott irányba, HAMIS, ha nem lehetséges a lépés
 */
bool MovePlayer(CellType ***map, Point *currentPosition, Point **boxPositions, Point direction, Move **movesListHead);
/**
 * Visszavonja a játékos előző lépést. Egészen addig fut le sikeresen,
 * amíg a moveList-ben volt elem, azaz meglépett lépés.
 * @param map A pályát leíró 2D-s dinamikus mátrix (cím szerint)
 * @param currentPosition A játékos aktuális koordinátája a pályán (map-en) (cím szerint)
 * @param boxPositions A dobozok koordinátáinak dinamikus tömbje cím szerint átadva (cím szerint)
 * @param moveListHead A játékos lépéseit eltároló láncolt lista cím szerint átadva (cím szerint)
 * @return
 */
bool UndoMove(CellType ***map, Point *currentPosition, Point **boxPositions, Move **moveListHead);

// == BEOLVASÁS ==
/**
 * Beolvassa a kapott fájlnévben lévő pályát és eltárolja a map mátrixban
 * @param filename (Bemenet)
 * @param map A pályát leíró 2D-s dinamikus mátrix (cím szerint, kimenet)
 * @param mapSize A pálya szélességét és magasságát leíró struktúra (cím szerint, kimenet)
 * @param playerPosition A játékos kezdő koordinátája a pályán (map-en) (cím szerint, kimenet)
 * @param boxPositions A dobozok koordinátáinak dinamikus tömbje (cím szerint, kimenet)
 * @param boxCount A dobozok koordinátáit tároló dinamikus tömb elemszáma, dobozok száma a pályán (cím szerint, kimenet)
 */
void ReadXSBFile(char filename[], CellType ***map, Size *mapSize, Point *playerPosition, Point **boxPositions, int *boxCount);
/**
 * A beolvasott fájl egy karakterét karaktereket értelmezi és átalakítja cellType értékké
 * @param character A beolvasott fájl egy karaktere
 * @return A kapott karakter értelmezett CellType értékekké alakított értéke
 */
CellType ConvertInputCharToCellType(char character);

// == KIIRATÁS ==
/**
 * Kiírja a bal felső sarokba a pályát. Csak tesztelésre való
 * @param map A pályát leíró 2D-s dinamikus mátrix
 * @param size A pálya szélességét és magasságát leíró struktúra
 */
void PrintSimpleMap(CellType **map, Size size);
/**
 * Kiiratja a pályát a képernyőre színesen
 * @param map A pályát leíró 2D-s dinamikus mátrix
 * @param size A pálya szélességét és magasságát leíró struktúra
 */
void PrintStyledMap(CellType **map, Size size);
/**
 * Egy kapott koordinátán lévő mezőt írja ki a képernyőre színesen és a megfelelő definiált karaterrel
 * @param map A pályát leíró 2D-s dinamikus mátrix
 * @param pos A kiirandó karakter koordinátája
 */
void PrintPosition(CellType **map, Point pos);
/**
 *
 * @param mapSize
 * @param numOfSteps
 */
void PrintStatsAndNav(Size mapSize, int numOfSteps, int level);
// == MEMÓRIAFOGLALÁS ÉS FELSZABADÍTÁS ==
/**
 * Memóriát foglal a pályát tároló 2D-s dinamikus mátrixnak (map-nek)
 * @param map A pályát leíró 2D-s dinamikus mátrix (cím szerint)
 * @param mapSize A pálya szélességét és magasságát leíró struktúra
 */
void AllocateMemoryToMap(CellType ***map, Size *mapSize);
/**
 * Memóriát foglal egy 1D-s dinamikus tömbnek
 * @param newArray Az új 1D-s dinamikus tömb címe (cím szerint)
 * @param lenght A létrehozandó dinamikus tömb hossza, elemszáma
 */
void AllocateDynamicArray(Point **newArray, int lenght);
/**
 * Felszabadítja a pályát tároló 2D-s dinamikus mátrix lefoglat memóriáját, ha volt lefoglalva
 * @param map A pályát leíró 2D-s dinamikus mátrix (cím szerint)
 */
void FreeAllocatedMemoryFromMap(CellType ***CellType);
/**
 * Felszabadítja egy 1D-s dinamikus tömbnek lefoglalt memóriáját, ha volt lefoglalva
 * @param dynamicArray 1D-s dinamikus tömb címe (cím szerint)
 */
void FreeDynamicArray(Point **dynamicArray);

#endif //GITIGNORE_GAME_H
