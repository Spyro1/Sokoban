#ifndef SOKOBAN2_MAIN_H
#define SOKOBAN2_MAIN_H

#include <stdlib.h>
#include "econio.h"
#include "datatypes.h"
// Konstansok
#define activeForeColor COL_LIGHTCYAN
#define activeBgColor COL_RESET
#define baseForeColor COL_LIGHTBLUE



//Függvények
void MainScreen();

void ResetMenuVars(bool *displayFirst, int *option, int *selectedPlayer);

void PrintTitle();

#endif
