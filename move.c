#include "move.h"
#include "debugmalloc.h"
#include "datatypes.h"

Move* MakeMove(Point stepfrom, Point stepTo, bool boxPushed){
    Move *newMove = (Move*) malloc(sizeof(Move));
    if (newMove == NULL){
        perror("Nem sikerult az uj Move lepesnek memoriat foglalni.");
        return NULL;
    }
    // Értékátadás
    newMove->from = stepfrom;
    newMove->to = stepTo;
    newMove->boxPushed = boxPushed;
    newMove->next = NULL;
    return newMove;
}
void AddMove(Move *newMove, Move **moveListHead){
    // Lista elejére beszúrás
    newMove->next = (struct Move *) *moveListHead;
    *moveListHead = newMove;
}

Move RemoveMove(Move **moveListHead){
    if (*moveListHead != NULL){
        Move *temp = (Move*) (*moveListHead)->next;
        Move removed = **moveListHead;
        FreeNode(moveListHead);
        *moveListHead = temp;
        return removed;
    }
    return (Move){-1,-1,NULL,-1,-1,NULL};
}

void FreeMoveList(Move **moveListHead){
    Move *temp;
    while (*moveListHead != NULL){
        temp = (Move*) (*moveListHead)->next;
        FreeNode(moveListHead);
        *moveListHead = temp;
    }
}
void FreeNode(Move **moveNode){
    if (moveNode != NULL){
        free(*moveNode);
    }
}
