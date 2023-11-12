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

#define waitms 700


//Függvények
/**
 * A főmenüt futtató függvény. Egyszer hivandó meg a mainben
 */
void MainScreen();

/**
 *
 * @param key
 * @param state
 * @param displayFirst Először megy-e be a ciklus a menüpontba tulajdonság
 * @param option  A kiválasztott opciók változója
 * @param selectedPlayer A kiválasztott játékos indexének változója
 * @param runMenu
 * @param currentPlayer
 * @param playerListHead
 * @param numOfPlayers
 * @param levelList
 */
void KeyPress(int key, State *state, bool *displayFirst, int *option, int *selectedPlayer, bool *runMenu, Player *currentPlayer, Player *playerListHead, int *numOfPlayers, char **levelList);

void PrintMainMenu(bool *displayFirst, int option, int prevOption, Point p);
void PrintExitWindow(bool runMenu, bool *displayFirst, int option, Point p);
void PrintNewPlayerSubMenu(State *state, Player *playerListHead, int *numOfPlayers, Point p);
void PrintPlayerSubMenu(bool *displayFirst, Player **playerListHead, int *numOfPlayers, Player **currentPlayer, int selectedPlayer, Point p );
void PrintRankList(bool *displayFirst, Player *playerListHead, int *numOfPlayers, Point p, Size maxSize);

/**
 * Visszaállítja a kezdőértékeket a menüben, ha menüpont váltás volt
 * @param displayFirst Először megy-e be a ciklus a menüpontba tulajdonság
 * @param option A kiválasztott opciók változója
 * @param selectedPlayer A kiválasztott játékos indexének változója
 */
void ResetMenuVars(bool *displayFirst, int *option, int *selectedPlayer);

/**
 * Kiirítja a főcímet a képernyő tetejére nagy betűkkel több sorosan, és alulra a navigációt
 */
void PrintTitle();

void WarningWindow(const char* Message, Point p, int option, bool *displayFirst, EconioColor baseColor, EconioColor accentForeColor, EconioColor accentBgColor);

#endif //SOKOBAN2_MENU_H
