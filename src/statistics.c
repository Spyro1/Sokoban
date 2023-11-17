#include "../headers/statistics.h"
#include "../libraries/debugmalloc.h"

void stats_AddLevelStatistics(int stepCount, Statistics **statsListHead){
    // Új statisztika létrehozása
    Statistics *newStat = (Statistics*) malloc(sizeof(Statistics));
    if (newStat == NULL){
        perror("Nem sikerult az uj Move lepesnek memoriat foglalni.");
        return;
    }
    // Értékátadás
    newStat->stepCount = stepCount;
    newStat->next = NULL;

    // Fejbe beszúrás ha üres al ista
    if (*statsListHead == NULL){
        *statsListHead = newStat;
    }
    else{
    // Lista végére beszúrás
        Statistics *mover = (Statistics *) *statsListHead;
        while (mover->next != NULL){
            mover = (Statistics *) mover->next;
        }
        mover->next = (struct Statistics *) newStat;
    }
}

//Move RemoveStat(Move **moveListHead){
//    if (*moveListHead != NULL){
//        Move *temp = (Move*) (*moveListHead)->next;
//        Move removed = **moveListHead;
//        move_FreeNode(moveListHead);
//        *moveListHead = temp;
//        return removed;
//    }
//    return (Move){-1,-1,NULL,-1,-1,NULL};
//}

void stats_FreeStatisticsList(Statistics **statsListHead){
    Statistics *temp;
    while (*statsListHead != NULL){
        temp = (Statistics*) (*statsListHead)->next;
        stats_FreeStatNode(statsListHead);
        *statsListHead = temp;
    }
}
static void stats_FreeStatNode(Statistics **statNode){
    if (*statNode != NULL){
        free(*statNode);
    }
}


