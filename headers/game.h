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
 * Ezzel kell meghívni a játékot. Inicializálja a játékhoz szükséges elemeket
 * @param player Player* Az aktuális játékos adatait tartalmazza (Cím szerint)
 * @param levelList char[][] A pályák fájlneveit tartalmazó string tömb
 * @return bool Igaz, ha a játékos teljesítette a szintet; Hamis, ha a játékos kilépett a játékból
 */
bool game_Init(Player *player, char **levelList);
/**
 * Ezzel a fügvénnyel indítja el és futtatja a játékot
 * @param player Az aktuális játékos adatait tartalmazza (Cím szerint)
 * @param levelName A betöltendő pálya fájlneve
 * @return Logikai: Igaz, ha a játékos teljesítette a szintet; Hamis, ha a játékos kilépett a játékból
 */
//bool game_StartGame(Player *player, char levelName[]);
/**
 * Ellenőrzi, hogy a játékos teljesítette-e a szintet, vagyis, hogy minden doboz a helyére került-e
 * @param map A pályát leíró 2D-s dinamikus mátrix
 * @param mapSize A pálya szélességét és magasságát leíró struktúra
 * @return Logikai: Igaz, ha minden doboz a helyére került; Hamis, ha van egy doboz is, ami nincs a helyén
 */
//bool game_CheckWin(CellType **map, Size mapSize);
 /**
 * A játékos elmozdulását tesztelő függvény a direction irányba.
 * Ha lehetséges a lépés, vagy ha doboz van a lépés irányában akkor a dobozt eltolja, és igazat ad vissza; Hamis, ha nem lehetséges a lépés
 * @param map A pályát leíró 2D-s dinamikus mátrix (Cím szerint)
 * @param mapSize A pálya szélességét és magasságát leíró struktúra
 * @param currentPosition A játékos aktuális koordinátája a pályán (map-en) (Cím szerint)
 * @param boxPositions A dobozok koordinátáinak dinamikus tömbje (Cím szerint)
 * @param direction A játékos elmozdulásvektora
 * @param movesListHead A játékos lépéseit eltároló láncolt lista (Cím szerint)
 * @return Logikai: Igaz, ha el tud mozdulni a játékos az adott irányba; Hamis, ha nem lehetséges a lépés
 */
//  bool game_MovePlayer(CellType ***map, Point *currentPosition, Point **boxPositions, Point direction, Move **movesListHead);
/**
 * Visszavonja a játékos előző lépést. Egészen addig fut le sikeresen,
 * amíg a moveList-ben volt elem, azaz meglépett lépés.
 * @param map A pályát leíró 2D-s dinamikus mátrix (Cím szerint)
 * @param currentPosition A játékos aktuális koordinátája a pályán (map-en) (Cím szerint)
 * @param boxPositions A dobozok koordinátáinak dinamikus tömbje (Cím szerint)
 * @param moveListHead A játékos lépéseit eltároló láncolt lista (Cím szerint)
 * @return Logikai: Igaz, ha sikeresen visszavonta a lépést; Hamis, ha nincs több visszavonható lépés
 */
// bool game_UndoMove(CellType ***map, Point *currentPosition, Point **boxPositions, Move **moveListHead);

// == BEOLVASÁS ==
/**
 * Beolvassa a kapott fájlnévben lévő pályát és eltárolja a map mátrixban
 * @param filename A pálya fájlneve (Bemenet)
 * @param map A pályát leíró 2D-s dinamikus mátrix (Cím szerint, kimenet)
 * @param mapSize A pálya szélességét és magasságát leíró struktúra (Cím szerint, kimenet)
 * @param playerPosition A játékos kezdő koordinátája a pályán (map-en) (Cím szerint, kimenet)
 * @param boxPositions A dobozok koordinátáinak dinamikus tömbje (Cím szerint, kimenet)
 * @param boxCount A dobozok koordinátáit tároló dinamikus tömb elemszáma, dobozok száma a pályán (Cím szerint, kimenet)
 */
// void game_ReadXSBFile(char filename[], CellType ***map, Size *mapSize, Point *playerPosition, Point **boxPositions, int *boxCount);
/**
 * A beolvasott fájl egy karakterét értelmezi és átalakítja cellType értékké
 * @param character A beolvasott fájl egy karaktere
 * @return A kapott karakter értelmezett CellType értékekké alakított értéke
 */
// CellType game_ConvertInputCharToCellType(char character);

// == KIIRATÁS ==
/**
 * Kiírja a bal felső sarokba a pályát. Csak tesztelésre való
 * @param map A pályát leíró 2D-s dinamikus mátrix
 * @param mapSize A pálya szélességét és magasságát leíró struktúra
 */
// void game_PrintSimpleMap(CellType **map, Size mapSize);
/**
 * Kiiratja a pályát a képernyőre színesen
 * @param map A pályát leíró 2D-s dinamikus mátrix
 * @param mapSize A pálya szélességét és magasságát leíró struktúra
 */
// void game_PrintStyledMap(CellType **map, Size mapSize);
/**
 * Egy kapott koordinátán lévő mezőt írja ki a képernyőre színesen és a megfelelő definiált karaterrel
 * @param map A pályát leíró 2D-s dinamikus mátrix
 * @param pos A kiirandó karakter koordinátája
 */
// void game_PrintPosition(CellType **map, Point pos);
/**
 * Kiírja a képernyőre a játéktér mellé az aktuális szintet és a lépések számát, vagy a tutorial pályánál a bevezető instrukciókat
 * @param mapSize A pálya szélességét és magasságát leíró struktúra
 * @param numOfSteps A szinten megtett lépések száma
 * @param level Az aktuális szint száma
 */
// void game_PrintStatsAndNav(Size mapSize, int numOfSteps, int level);

// == MEMÓRIAFOGLALÁS ÉS FELSZABADÍTÁS ==
/**
 * Memóriát foglal a pályát tároló 2D-s dinamikus mátrixnak (map-nek)
 * @param map A pályát leíró 2D-s dinamikus mátrix (Cím szerint)
 * @param mapSize A pálya szélességét és magasságát leíró struktúra
 */
// void game_AllocateMemoryToMap(CellType ***map, Size *mapSize);
/**
 * Memóriát foglal egy 1D-s dinamikus tömbnek
 * @param newArray Az új 1D-s dinamikus tömb címe (Cím szerint)
 * @param lenght A létrehozandó dinamikus tömb hossza, elemszáma
 */
// void game_AllocateDynamicArray(Point **newArray, int lenght);
/**
 * Felszabadítja a pályát tároló 2D-s dinamikus mátrix lefoglat memóriáját, ha volt lefoglalva
 * @param map A pályát leíró 2D-s dinamikus mátrix (Cím szerint)
 */
// void game_FreeAllocatedMemoryFromMap(CellType ***map);
/**
 * Felszabadítja egy 1D-s dinamikus tömbnek lefoglalt memóriáját, ha volt lefoglalva
 * @param dynamicArray 1D-s dinamikus tömb címe (Cím szerint)
 */
// void game_FreeDynamicArray(Point **dynamicArray);

#endif //GITIGNORE_GAME_H
