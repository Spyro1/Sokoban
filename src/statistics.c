#include "../headers/statistics.h"
#include "../libraries/debugmalloc.h"
#include "../headers/lib.h"

void stats_AddLevelStatistics(int stepCount, Statistics **statsListHead){
    // Új statisztika létrehozása
    Statistics *newStat = (Statistics*) malloc(sizeof(Statistics));
    if (newStat == NULL){
        lib_printError("Nem sikerült az új Move lépésnek memóriat foglalni.");
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

void stats_FreeStatisticsList(Statistics **statsListHead){
    Statistics *temp;
    while (*statsListHead != NULL){
        temp = (Statistics*) (*statsListHead)->next;
        stats_FreeStatNode(statsListHead);
        *statsListHead = temp;
    }
} void stats_FreeStatNode(Statistics **statNode){
    if (*statNode != NULL){
        free(*statNode);
    }
}


