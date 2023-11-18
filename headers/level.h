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
void level_ReadDirectory(char directory[], char **levelList[], int *numOfFiles);
/**
 *
 * @param levelList
 * @param numOfLevels
 */
void level_FreeLevelList(char ***levelList, int *numOfLevels);


#endif //SOKOBAN2_LEVEL_H
