#include <stdio.h>
#include <string.h>
#include "datatypes.h"
#include "econio.h"
#include "main.h"
#include "debugmalloc.h"
#include "player.h"
#include <dirent.h>
#include <string.h>

int const nameLenght = 21;

void player_ReadTxtFile(Player **playerList, int *numOfPlayers) {
    *numOfPlayers = 0;
    FILE *fp;
    fp = fopen("./players.txt", "r");
    if (fp == NULL) {
        printf("Nem lehet megnyitni a fájlt\n");
    }
    else {
        char inputLine[30], name[nameLenght];
        int completedLevels, totalMoves, averageMoves;

        while (fgets(inputLine, 30, fp)) {
            // %20 helyére a (int const) nameLenght értékét mindig
            if (sscanf(inputLine, "%21[^;];%d;%d;%d", name, &completedLevels,&totalMoves,&averageMoves) == 4){
                player_AddPlayer(player_MakePlayer(name, completedLevels, totalMoves, averageMoves), playerList, numOfPlayers);
            }
        }
    }
    fclose(fp);
    // New Player
    player_AddPlayer(player_MakePlayer("Új Játékos", 0, 0,0), playerList, numOfPlayers);
}

void player_FreePlayerList(Player **playerList){
    while (*playerList != NULL) {
        Player *temp = (Player*) (*playerList)->next;
        free(*playerList);
        *playerList = temp;
    }
}

Player *player_MakePlayer(char name[], int completedLevels, int totalMoves, int averageMoves){
    Player *uj = (Player *) malloc(sizeof(Player));
    strcpy(uj->name, name);
    uj->completedLevels = completedLevels;
    uj->totalMoves = totalMoves;
    uj->averageMoves = averageMoves;
    uj->next = NULL;
    //uj->back = NULL;
    return uj;
}

void player_AddPlayer(Player *newPlayer, Player **playerList, int *numOfPlayers){
    //if (*playerList != NULL) (*playerList)->back = (struct Player*) newPlayer;
    newPlayer->next = (struct Player*) *playerList;
    //newPlayer->back = NULL;
    *playerList = newPlayer;
    (*numOfPlayers)++;
}

void player_RemovePlayer(Player *removablePlayer, Player **playerList, int *numOfPlayers){
    // Store Head of the List
    Player *temp = *playerList, *prev;
    // Player Keresése
    // Ha az első elem:
    if (temp != NULL && temp->name == removablePlayer->name) {
        *playerList = (Player *) temp->next; // Changed head
        free(temp); // free old head
        return;
    }
    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && temp->name != removablePlayer->name) {
        prev = temp;
        temp = (Player *) temp->next;
    }

    // If key was not present in linked list
    if (temp == NULL)
        return;

    // Unlink the node from linked list
    prev->next = temp->next;

    free(temp); // Free memory

//    while((*playerList)->next != NULL && *playerList != removablePlayer){
//        *playerList = (Player*) (*playerList)->next;
//    }
//    // Kiemelés
//    Player *backNeighbour = (Player *) (*playerList)->back;
//    Player *nextNeighbour = (Player *) (*playerList)->next;
//    backNeighbour->next = (struct Player *) nextNeighbour;
//    nextNeighbour->back = (struct Player *)  backNeighbour;


    //Visszaléptetés a lista elejére
//    while((*playerList)->back != NULL){
//        *playerList = (Player*) (*playerList)->back;
//    }
}

void player_PrintPlayerList(Player *playerList, int numOfPlayers, int selectedPlayerIndex, Point start){
    int currentIndex = 0;
    //ClearScreenSection(0, 8, 60, 19, COL_RESET);
    while(playerList != NULL){
        econio_gotoxy(start.x, start.y + currentIndex);
        if (currentIndex == selectedPlayerIndex){
            econio_textcolor(COL_BLUE);
            econio_textbackground(COL_LIGHTCYAN);
        }
        else {
            econio_textcolor(COL_LIGHTCYAN);
            econio_textbackground(COL_RESET);
        }
        if (playerList->completedLevels == 0) printf("                  + %s +                  ", playerList->name);
        else printf("%20s,  Szint: %2d, Átlag: %2d  lépés", playerList->name, playerList->completedLevels, playerList->averageMoves);
        playerList = (Player*) playerList->next;
        currentIndex++;
        econio_textbackground(COL_RESET);
    }
}

Player* player_GetSelectedPlayer(Player *playerList, int selectedPlayer){
    if (playerList != NULL){
        for (int i = 0; i < selectedPlayer; i++){
        //        Player *temp = (Player*) playerList->next;
        //        playerList = temp;
            playerList = (Player*) playerList->next;
        }
    }
    return playerList;
}