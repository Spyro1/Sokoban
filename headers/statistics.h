#ifndef SOKOBAN2_STATISTICS_H
#define SOKOBAN2_STATISTICS_H
#include "datatypes.h"

/**
 * Beszúrja a paraméterként kapott stepCount értéket a statsListHead láncolt lista végére
 * @param stepCount int A szinten megtett lépések száma
 * @param statsListHead Statistics** A lépések számát tároló láncolt lista (Cím szerint)
 */
void stats_AddLevelStatistics(int stepCount, Statistics **statsListHead);
/**
 * Felszabadítja a az egész láncolt listának foglalt memóriát
 * @param statsListHead Statistics**  A lépések számát tároló láncolt lista (Cím szerint)
 */
void stats_FreeStatisticsList(Statistics **statsListHead);
/**
 * Felszabadítja egy elem lefoglalt memóriáját a listából
 * @param statNode Statistics** Egy Statistics struktúrára mutató pointer a láncolt listából (Cím szerint)
 */
void stats_FreeStatNode(Statistics **statNode);

#endif //SOKOBAN2_STATISTICS_H
