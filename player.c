#include <stdio.h>
#include <string.h>
#include "datatypes.h"
#include "econio.h"
#include "main.h"
#include "debugmalloc.h"
#include "player.h"
#include <dirent.h>
#include <string.h>



void player_ReadTxtFile(Player **playerList, int *numOfPlayers) {
    *numOfPlayers = 0;
    FILE *fp;
    fp = fopen("./players.txt", "r");
    if (fp == NULL) {
        printf("Nem lehet megnyitni a fájlt\n");
    }
    else {
        char inputLine[maxReadLineLenght], name[nameLenght], inputRest[maxReadLineLenght];
        int completedLevels;
        int *levelMoves;

        while (fgets(inputLine, maxReadLineLenght, fp)) {
            // %20 helyére a (int const) nameLenght értékét mindig
            if (sscanf(inputLine, "%20[^;];%d;%s[^\n]", name, &completedLevels, inputRest) == 3){
                levelMoves = (int*) malloc(completedLevels * sizeof(int)); // Memóriafoglalás
                player_AddPlayer(player_MakePlayer(name, completedLevels, levelMoves), playerList, numOfPlayers);
            }
        }
    }
    fclose(fp);
    // New Player
    //player_AddPlayer(player_MakePlayer("Új Játékos", 0, ), playerList, numOfPlayers);
}

void player_FreePlayerList(Player **playerList){
    while (*playerList != NULL) {
        Player *temp = (Player*) (*playerList)->next;
        free((*playerList)->levelMoves);
        free(*playerList);
        *playerList = temp;
    }
}

Player *player_MakePlayer(char name[], int completedLevels, const int *levelMoves){
    Player *uj = (Player *) malloc(sizeof(Player));
    strcpy(uj->name, name);
    uj->completedLevels = completedLevels;
    for (int i = 0; i < completedLevels; i++){
        uj->levelMoves[i] = levelMoves[i];
    }
    uj->next = NULL;
    //uj->back = NULL;
    return uj;
}

void player_AddPlayer(Player *newPlayer, Player **playerListHead, int *numOfPlayers){
    if (*playerListHead == NULL){

    }
    Player *mover = *playerListHead;

    newPlayer->next = (struct Player*) *playerListHead;
    *playerListHead = newPlayer;
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

void player_PrintPlayerList(Player *playerList, int selectedPlayerIndex, Point center){
    int currentIndex = 0;
    //ClearScreenSection(0, 8, 60, 19, COL_RESET);
    while(playerList != NULL){
        char *text = NULL;
        sprintf(text,"%20s, Szint: %2d", playerList->name, playerList->completedLevels);
        if (currentIndex == selectedPlayerIndex)
            printfbc(text,center.x - (int)strlen(text),center.y + currentIndex, activeForeColor, activeBgColor);
        else
            printfc(text,center.x - (int)strlen(text),center.y + currentIndex, baseForeColor);
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