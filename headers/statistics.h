#ifndef SOKOBAN2_STATISTICS_H
#define SOKOBAN2_STATISTICS_H
#include "datatypes.h"

void AddLevelStatistics(int stepCount, Statistics **statsListHead);

void FreeStatisticsList(Statistics **statsListHead);

void FreeStatNode(Statistics **statNode);

#endif //SOKOBAN2_STATISTICS_H
