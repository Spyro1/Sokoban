#ifndef SOKOBAN2_MOVE_H
#define SOKOBAN2_MOVE_H

#include "datatypes.h"
#include <stdbool.h>


Move* MakeMove(Point stepfrom, Point stepTo, bool boxPushed);
void AddMove(Move *newMove, Move **moveListHead);
Move RemoveMove(Move **moveListHead);
void FreeMoveList(Move **statsListHead);
void FreeNode(Move **moveNode);
#endif //SOKOBAN2_MOVE_H
