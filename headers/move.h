#ifndef SOKOBAN2_MOVE_H
#define SOKOBAN2_MOVE_H

#include <stdbool.h>
#include "datatypes.h"

/**
 * Létrehoz egy Move struktúrára mutató pointert a paraméterként kapott értékekből, hogy aztán Lístába lehessen fűzni.
 * @param stepfrom A legutolsó pozíció koordinátája
 * @param stepTo A következő pozíció koordinátája
 * @param boxPushed Logikai: IGAZ, ha eltolt doboz, HAMIS, ha csak a játékos mozdult el
 * @return Move strúktúrára mutató pointer a kapott paraméterekkel
 */
Move* move_CreateMove(Point stepfrom, Point stepTo, bool boxPushed);
/**
 * Beszúrja a paraméterként kapott newMove elemet a láncolt lista (moveListHead) elejére
 * @param newMove Új elmozdulást tároló struktúrára mutató pointer
 * @param moveListHead Az elmozdulásokat tároló láncolt lista (Verem/Stack)
 */
void move_AddMoveToList(Move *newMove, Move **moveListHead);
/**
 * Eltávolítja az első elemet a láncolt listából (Veremből/Stack)
 * @param moveListHead Az elmozdulásokat tároló láncolt lista (Verem/Stack) (Cím szerint)
 * @return Visszaadja az eltávolított listaelem struktúráját
 */
Move move_RemoveMoveFromList(Move **moveListHead);
/**
 * Felszabadítja a az egész láncolt listának foglalt memóriát
 * @param moveListHead Az elmozdulásokat tároló láncolt lista (Verem/Stack) (Cím szerint)
 */
void move_FreeMoveList(Move **moveListHead);
/**
 * Felszabadítja egy elem lefoglalt memóriáját a listából
 * @param moveNode Egy Move struktúrára mutató pointer a láncolt listából (Cím szerint)
 */
static void move_FreeNode(Move **moveNode);

#endif //SOKOBAN2_MOVE_H