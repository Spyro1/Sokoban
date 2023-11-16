#ifndef SOKOBAN2_LEVEL_H
#define SOKOBAN2_LEVEL_H

#include <stdlib.h>
#include "../libraries/econio.h"
#include "datatypes.h"

/**
 *
 * @param directory
 * @param levelList
 * @param numOfFiles
 */
void ReadDirectoryLevelNames(char directory[], char **levelList[], int *numOfFiles);
/**
 *
 * @param levelList
 * @param numOfLevels
 */
void FreeLevelList(char ***levelList, const int *numOfLevels);
/**
 *
 * @param levelList
 * @param numOfLevels
 * @param selectedLevel
 * @param maxLevels
 * @param start
 */
void PrintLevels(char **levelList, int numOfLevels, int selectedLevel, int maxLevels, Point start);


#endif //SOKOBAN2_LEVEL_H
