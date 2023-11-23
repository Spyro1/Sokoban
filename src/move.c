#include "../libraries/debugmalloc.h"
#include "../headers/datatypes.h"
#include "../headers/move.h"

Move* move_CreateMove(Point stepfrom, Point stepTo, bool boxPushed){
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
void move_AddMoveToList(Move *newMove, Move **moveListHead){
    // Lista elejére beszúrás
    newMove->next = (struct Move *) *moveListHead;
    *moveListHead = newMove;
}

Move move_RemoveMoveFromList(Move **moveListHead){
    if (*moveListHead != NULL){
        Move *temp = (Move*) (*moveListHead)->next;
        Move removed = **moveListHead;
        move_FreeNode(moveListHead);
        *moveListHead = temp;
        return removed;
    }
    Move temp = {{-1,-1},{-1,-1}, false};
    return temp;
}

void move_FreeMoveList(Move **moveListHead){
    Move *temp;
    while (*moveListHead != NULL){
        temp = (Move*) (*moveListHead)->next;
        move_FreeNode(moveListHead);
        *moveListHead = temp;
    }
}
 void move_FreeNode(Move **moveNode){
    if (moveNode != NULL){
        free(*moveNode);
    }
}
