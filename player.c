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
    fp = fopen(playerDataPath, "r");
    if (fp == NULL) {
        printf("Nem lehet megnyitni a fájlt\n");
    }
    else {
        player_FreePlayerList(playerList); // Ha van lefoglalva már, akkor felszabadítja a listát
        char inputLine[maxReadLineLenght], name[nameLenght], inputRest[maxReadLineLenght];
        int completedLevels;
        int *levelMoves = NULL;
        int i;
        while (fgets(inputLine, maxReadLineLenght, fp)) {
            // %20 helyére a (int const) nameLenght értékét mindig
            int args = sscanf(inputLine, "%20[^;];%d;%s[^\0]", name, &completedLevels, inputRest);
            if (args == 3){
                levelMoves = (int*) malloc((completedLevels+1) * sizeof(int)); // Memóriafoglalás
                if (levelMoves == NULL){
                    perror("Nem sikerult memorit foglalni a beolvasott player.txt soranak levelMoves tombjenek.");
                }
                else{
                    if (completedLevels == 1){
                        sscanf(inputRest, "%d", &levelMoves[i]);
                    }
                    else if (completedLevels == 2){
                        sscanf(inputRest, "%d;%d", &levelMoves[i], &levelMoves[i+1]);
                    }
                    else{
                        int actualNumOfCompletedLevels = completedLevels;
                        for(i = 0; i < completedLevels-1; i++){
                            if (sscanf(inputRest, "%d;%s[^\n]", &levelMoves[i], inputRest) != 2){
                                actualNumOfCompletedLevels--;
                            }
                        }
                        sscanf(inputRest, "%d", &levelMoves[i]);
                        completedLevels = actualNumOfCompletedLevels;
    //                if (sscanf(inputRest, "%d;%d[^\n]", &levelMoves[i], &levelMoves[i+1]) != 2){
    //                    levelMoves[i] = -1; // Nincs bemenet
    //                    levelMoves[i+1] = -1; // Nincs bemenet
    //                }
                    }
                }
                player_AddPlayerToEnd(player_MakePlayer(name, completedLevels, levelMoves), playerList, numOfPlayers);
            }
            else if (args == 2){
                levelMoves = (int*) malloc((completedLevels+1) * sizeof(int)); // Memóriafoglalás
                player_AddPlayerToEnd(player_MakePlayer(name, completedLevels, levelMoves), playerList, numOfPlayers);
            }
            else if (*playerList == NULL) {
                return;
            }
            else{
                perror("Hiba a player.txt beolvasasanal, nem megfelelo bemenet!");
            }
        }
    }
    fclose(fp);
    // New Player
    //player_AddPlayerToEnd(player_MakePlayer("Új Játékos", 0, ), playerList, numOfPlayers);
}

void player_WriteTxtFile(Player *playerListHead, int numOfPlayers){
    FILE *fp = fopen(playerDataPath, "wt");
    if (fp == NULL){
        perror("Nem sikerult megnyitni a player.txt fajlt.");
        return;
    }
    char printer[maxReadLineLenght];
    for(Player *mover = playerListHead; mover != NULL; mover = (Player *) mover->next){
        sprintf(printer,"%s;%d", mover->name, mover->numOfCompletedLevels);
        for(int i = 0; i < mover->numOfCompletedLevels; i++){
            sprintf(printer, "%s;%d", printer, mover->levelMoves[i]);
        }
        fprintf(fp, "%s\n", printer);
    }
    fclose(fp);
}

void player_FreePlayerList(Player **playerList){
    Player *temp;
    while (*playerList != NULL) {
        temp = (Player*) (*playerList)->next;
        player_FreePlayerNode(playerList);
        *playerList = temp;
    }
}
void player_FreePlayerNode(Player **playerNode){
    if (*playerNode != NULL){
        if ((*playerNode)->levelMoves != NULL)
            free((*playerNode)->levelMoves);
        free(*playerNode);
    }
}
/**
 *
 * @param name
 * @param numOfLevels
 * @param levelMoves
 * @return
 */
Player *player_MakePlayer(char name[], int numOfLevels, int *levelMoves){
    // Memóriafoglalás
    Player *uj = (Player *) malloc(sizeof(Player));
    if (uj == NULL){
        perror("Nem sikerult az uj Player node-nak memoriat foglalni.");
        return NULL;
    }
    // Adatok átadása
    strcpy( uj->name, name); // Név
    uj->numOfCompletedLevels = numOfLevels; // Teljesített szintek Száma
    uj->levelMoves = levelMoves;
    uj->next = NULL;
    return uj;
}
/**
 *
 * @param newPlayer
 * @param playerListHead
 * @param numOfPlayers
 */
void player_AddPlayerToEnd(Player *newPlayer, Player **playerListHead, int *numOfPlayers){
    if (*playerListHead == NULL){
        *playerListHead = newPlayer;
    }
    else{
        Player *lastNode = *playerListHead;
        while (lastNode->next != NULL) {
            lastNode = (Player *) lastNode->next; // Végigfut a listán
        }
        lastNode->next = (struct Player *) newPlayer; // Beillesztés a végére
    }
    (*numOfPlayers)++;
}

bool player_RemovePlayer(Player *removablePlayer, Player **playerListHead, int *numOfPlayers){
    // A Lista elejének eltárolása
    Player *temp = *playerListHead, *prev;
    // Player Keresése
    // Ha az első elem a keresett
    if (temp != NULL && temp->name == removablePlayer->name) {
        *playerListHead = (Player *) temp->next; // Első elem továbbléptetése
        player_FreePlayerNode(&temp); // Törlés
        return true; // Sikeres törlés
    }
    // removablePlayer keresése a bév alapján
    while (temp != NULL && temp->name != removablePlayer->name) {
        prev = temp;
        temp = (Player *) temp->next;
    }
    // Ha nem talált a listában
    if (temp == NULL) {
        return false;
    }
    else {
        prev->next = temp->next; // Mutató átmozgatása a következő elemre
        player_FreePlayerNode(&temp);
        return true;
    }
}

void player_PrintPlayerList(Player *playerList, int selectedPlayerIndex, Point center){
    int currentIndex = 0;
    //ClearScreenSection(0, 8, 60, 19, COL_RESET);
    if (playerList == NULL){
        printfc("Nincs egy játékos sem még.", center.x - (int)strlen("Nincs egy játékos sem még.")/2,center.y + currentIndex, baseForeColor );
    }
    while(playerList != NULL){
        char text[maxReadLineLenght];
        sprintf(text,"%s, Szint: %d", playerList->name, playerList->numOfCompletedLevels);
        if (currentIndex == selectedPlayerIndex)
            printfbc(text,center.x - (int)strlen(text)/2,center.y + currentIndex, activeForeColor, activeBgColor);
        else
            printfc(text,center.x - (int)strlen(text)/2,center.y + currentIndex, baseForeColor);
        playerList = (Player*) playerList->next;
        currentIndex++;
    }

}
void PrintRankList(Player *playerList, int numOfPlayer, Point p){
    Player *mover = playerList;
    int *spaces = (int*) malloc(numOfPlayer * sizeof(int));
    int indent = 8;
    int line = 0, prevline = 0;
    int namelenght = 0;
    printfc("╔═══════╦═",p.x, p.y, baseForeColor);
    printfc("║ Szint ║ ",p.x, p.y+1, baseForeColor);
    printfc("╠═══════╬═ ",p.x, p.y+2, baseForeColor);
    char text[nameLenght+5], helper[nameLenght];
    while (mover != NULL){
        line = 2;
        namelenght = (int) strlen(mover->name);
        for(int i = 0; i < namelenght + 1; i++) printfc("═", p.x+indent+i+2, p.y, baseForeColor);
        sprintf(text, "║ %s ║", mover->name); // Eltolás még eggyel nagypapa sornál
        printfc("╦", p.x+namelenght+indent+3, p.y, baseForeColor);
        printfc(text, p.x+indent, p.y+1, baseForeColor);
        for(int i = 0; i < namelenght + 1; i++) printfc("═", p.x+indent+i+2, p.y+2, baseForeColor);
        printfc("╬", p.x+namelenght+indent+3, p.y+2, baseForeColor);
        // Szint számának kiiratása
        line++;
        for (int i = 0; i < mover->numOfCompletedLevels; ++i) {
            sprintf(text, "║ %3d.  ║", i+1);
            printfc(text, p.x, p.y+i+3, baseForeColor);
            strcpy(text, "║ %");
            sprintf(helper, "%d\0", namelenght-2);
            strcat(text, helper);
            strcat(text,"dl  ║\0");
            sprintf(text, text, mover->levelMoves[i]);// Nagypapa sprnál hiba itt
            printfc(text, p.x+indent, p.y+i+3, baseForeColor);
            line++;
        }
        indent += 2 + namelenght;
        mover = (Player *) mover->next;
        prevline = line > prevline ? line : prevline;
    }

    printfc("╚═══════╩═",p.x,p.y+line,COL_CYAN);
    for(int i = 0; i < namelenght + 1; i++) printfc("═", p.x+10+i, p.y+line, baseForeColor);
    print("╗", p.x+indent+2, p.y);
    for(int i = 1; i < line; i++) printfc("║", p.x+indent+2, p.y+i, baseForeColor);
    print("╝", p.x+indent+2, p.y+line);
    free(spaces);
}

Player* player_GetSelectedPlayer(Player *playerList, int selectedPlayer){
    if (playerList != NULL){
        for (int i = 0; i < selectedPlayer; i++){
            playerList = (Player*) playerList->next;
        }
        return playerList;
    }
    else{
        return NULL;
    }
}
int player_GetIndexOfPlayer(Player *playerListHead, char name[]){
    Player *mover = playerListHead;
    int index = 0;
    while(mover != NULL && strcmp(mover->name, name) != 0){
        mover = (Player *) mover->next;
    }
    return mover != NULL ? index : -1;
}