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
void PrintTitle();
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
void KeyPress(int key, State *state, bool *displayFirst, int *option, int *selectedPlayer, bool *runMenu, Player *currentPlayer, Player **playerListHead, int *numOfPlayers, char **levelList);

void PrintExitWindow(bool runMenu, bool *displayFirst, int option, Point p);
void PrintMainMenu(bool *displayFirst, int option, int prevOption, Point p);
void PrintNewPlayerSubMenu(State *state, Player **playerListHead, int *numOfPlayers, int selectedPlayer, Point p);
void PrintPlayerSubMenu(bool *displayFirst, Player **playerListHead, int *numOfPlayers, Player **currentPlayer, int selectedPlayer, Point p );
void PrintRankList(bool *displayFirst, Player *playerListHead, int *numOfPlayers, Point p, int maxDisplayLvls);

/**
 * Visszaállítja a kezdőértékeket a menüben, ha menüpont váltás volt
 * @param displayFirst Először megy-e be a ciklus a menüpontba tulajdonság
 * @param option A kiválasztott opciók változója
 * @param selectedPlayer A kiválasztott játékos indexének változója
 */
void ResetMenuVars(bool *displayFirst, int *option, int *selectedPlayer);

void WarningWindow(const char* Message, Point p, int option, bool *displayFirst, EconioColor baseColor, EconioColor accentForeColor, EconioColor accentBgColor);

void PrintNavControls(State state,  Point p, Size maxSize);
#endif //SOKOBAN2_MENU_H
