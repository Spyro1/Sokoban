#ifndef SOKOBAN2_LEVEL_H
#define SOKOBAN2_LEVEL_H

#include <stdlib.h>
#include "econio.h"
#include "datatypes.h"

void ReadDirectoryLevelNames(char directory[], char **levelList[], int *numOfFiles);

void FreeLevelList(char ***levelList, const int *numOfLevels);

void PrintLevels(char **levelList, int numOfLevels, int selectedLevel, int maxLevels, Point start);


#endif //SOKOBAN2_LEVEL_H
