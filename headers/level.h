#ifndef SOKOBAN2_LEVEL_H
#define SOKOBAN2_LEVEL_H

#include <stdlib.h>
#include "../libraries/econio.h"
#include "datatypes.h"

/**
 * Beolvassa a megadott mappából a fájlneveket, és eltárolja egy dinamikusan foglalt tömbben
 * @param directory char[] A mappa elérési útvonala
 * @param levelList *char[][] A dinamikusan foglalt string tömb (Cím szerint)
 * @param numOfFiles int* A Beolvasott fájlnevek száma (Cím szerint)
 */
void level_ReadDirectory(char directory[], char **levelList[], int *numOfFiles);
/**
 * Felszabadítja a dinamikusan foglalt fájlnevek string tömbjét
 * @param levelList *char[][] A dinamikusan foglalt string tömb (Cím szerint)
 * @param numOfLevels int* A Beolvasott fájlnevek száma (Cím szerint)
 */
void level_FreeLevelList(char **levelList[], int *numOfLevels);

#endif //SOKOBAN2_LEVEL_H