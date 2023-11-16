#ifndef SOKOBAN2_MENU_H
#define SOKOBAN2_MENU_H

#include <stdlib.h>
#include "../libraries/econio.h"
#include "datatypes.h"

// Makrók
#define strNewPlayer "ÚJ JÁTÉKOS"
#define strChosePlayer "JÁTÉK INDÍTÁSA"
#define strRankList "DICSŐSÉGLISTA"
#define strExitApp "KILÉPÉS"

#define waitms 600
#define maxDisplayLines 10


//Függvények
/**
 * A főmenüt futtató függvény. Egyszer hivandó meg a mainben
 */
void MainScreen();


/**
 * Kiirítja a főcímet a képernyő tetejére nagy betűkkel több sorosan
 */
static void PrintTitle();
/**
 * Kiértékeli a felhasználó által lenyomott billentyőt, és megváltoztatja a program állapotát aszerint
 * @param key A lenyomott billentyű kódja
 * @param state A program állapotát tároló érték (Cím szerint)
 * @param displayFirst Először megy-e be a ciklus a menüpontba tulajdonság (Cím szerint)
 * @param option  A kiválasztott opciók változója (Cím szerint)
 * @param selectedPlayer A kiválasztott játékos indexének változója (Cím szerint)
 * @param runMenu Menü futtatását beállító logikai változó (Cím szerint)
 * @param currentPlayer Az kiválasztott játékos struktúrájára mutató pointer, playerList elem
 * @param playerListHead A játékosok adatait tartalmazó láncolt lista (Cím szerint)
 * @param numOfPlayers  A játékosok darabszáma (Cím szerint)
 * @param levelList A szintek fájlnevét tároló dinamikus string tömb
 */
static void KeyPress(int key, State *state, bool *displayFirst, int *option, int *selectedPlayer, bool *runMenu, Player *currentPlayer, Player **playerListHead, int *numOfPlayers, char **levelList);
/**
 *
 * @param runMenu
 * @param displayFirst
 * @param option
 * @param p
 */
static void PrintExitWindow(bool runMenu, bool *displayFirst, int option, Point p);
static void PrintMainMenu(bool *displayFirst, int option, int prevOption, Point p);
static void PrintNewPlayerSubMenu(State *state, Player **playerListHead, int *numOfPlayers, int selectedPlayer, Point p);
static void PrintPlayerSubMenu(bool *displayFirst, Player **playerListHead, int *numOfPlayers, Player **currentPlayer, int selectedPlayer, Point p );
static void PrintRankList(bool *displayFirst, Player *playerListHead, int *numOfPlayers, Point p, int maxDisplayLvls);

/**
 * Visszaállítja a kezdőértékeket a menüben, ha menüpont váltás volt
 * @param displayFirst Először megy-e be a ciklus a menüpontba tulajdonság
 * @param option A kiválasztott opciók változója
 * @param selectedPlayer A kiválasztott játékos indexének változója
 */
static void ResetMenuVars(bool *displayFirst, int *option, int *selectedPlayer);
/**
 *
 * @param Message
 * @param p
 * @param displayFirst
 * @param option
 * @param baseColor
 * @param accentForeColor
 * @param accentBgColor
 */
void WarningWindow(const char* Message, Point p, bool *displayFirst,int option,  EconioColor baseColor, EconioColor accentForeColor, EconioColor accentBgColor);

void PrintNavControls(State state,  Point p, Size maxSize);
#endif //SOKOBAN2_MENU_H
