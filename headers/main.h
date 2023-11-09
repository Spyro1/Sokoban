#ifndef SOKOBAN2_MAIN_H
#define SOKOBAN2_MAIN_H

#include <stdlib.h>
#include "../libraries/econio.h"
#include "datatypes.h"
// Konstansok
#define activeForeColor COL_LIGHTCYAN
#define activeBgColor COL_RESET
#define baseForeColor COL_LIGHTBLUE



//Függvények
/**
 * A főmenüt futtató függvény. Egyszer hivandó meg a mainben
 */
void MainScreen();
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

#endif
