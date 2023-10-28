#ifndef SOKOBAN2_MAIN_H
#define SOKOBAN2_MAIN_H

#include <stdlib.h>
#include "econio.h"
#include "datatypes.h"

//Függvények
void MainScreen(Player *currentPlayer, int numOfLevels, int *selectedLevel);

void ClearScrBellow();
void ClearScreenSection(int x1, int y1, int x2, int y2, EconioColor bgColor);




#endif
