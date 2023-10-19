#ifndef SOKOBAN2_MAIN_H
#define SOKOBAN2_MAIN_H

#include <stdlib.h>
#include "econio.h"
#include "datatypes.h"

//Függvények
void MainScreen(Player **PlayerList, char selectedLevelFileName[]);

void ClearScrBellow();
void ClearScreenSection(int x1, int y1, int x2, int y2, EconioColor bgColor);

void ReadDirectoryLevelNames(char directory[], char **levelList[], int *numOfFiles);

void FreeLevelList(char ***levelList, int numOfLevels);

void PrintLevels(char **levelList, int numOfLevels, int selectedLevel, int maxLevels, Point start);



#endif
