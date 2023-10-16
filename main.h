#ifndef SOKOBAN2_MAIN_H
#define SOKOBAN2_MAIN_H
#endif

#include <stdlib.h>
#include "econio.h"
#include "datatypes.h"

// Függvények
void MainScreen(int *selectedPlayer, int *selectedLevel, Player **players);

void PrintMap(char const **map, int width, int height);

void ReadXSBFile(char const filename[], char ***map);

void ClearScreenSection(int x1, int y1, int x2, int y2, EconioColor bgColor);

