#include <stdio.h>
#include <string.h>
#include <string.h>
#include "../libraries/debugmalloc.h"
#include "../libraries/econio.h"
#include "../headers/player.h"
#include "../headers/statistics.h"
#include "../headers/datatypes.h"



void player_ReadTxtFile(Player **playerList, int *numOfPlayers) {
    *numOfPlayers = 0;
    FILE *fp;
    fp = fopen(playerDataPath, "r");
    if (fp == NULL) {
        printf("Nem lehet megnyitni a fájlt\n");
    }
    else {
        player_FreePlayerList(playerList); // Ha van lefoglalva már, akkor felszabadítja a listát
        char inputLine[maxLineLenght], name[nameLenght], inputRest[maxLineLenght];
        int completedLevels;
        int stepCount;
        int i;
        while (fgets(inputLine, maxLineLenght, fp)) {
            Statistics *statsListHead = NULL;
            // %20 helyére a (int const) nameLenght értékét mindig
            int args = sscanf(inputLine, "%20[^;];%d;%s[^\n]", name, &completedLevels, inputRest);
            if (args == 3){
                //levelMoves = (int*) malloc((completedLevels+1) * sizeof(int)); // Memóriafoglalás
//                if (levelMoves == NULL){
//                    perror("Nem sikerult memorit foglalni a beolvasott player.txt soranak levelMoves tombjenek.");
//                }
//                else{
                    int actualNumOfCompletedLevels = completedLevels;
                    for(i = 0; i < completedLevels-1; i++){
                        if (sscanf(inputRest, "%d;%s[^\n]", &stepCount, inputRest) == 2)
                            AddLevelStatistics(stepCount, &statsListHead);
                        else
                            actualNumOfCompletedLevels--;

                    }
                    sscanf(inputRest, "%d", &stepCount);
                    AddLevelStatistics(stepCount, &statsListHead);
                    completedLevels = actualNumOfCompletedLevels;
//                }
                player_AddPlayerToEnd(player_MakePlayer(name, completedLevels, statsListHead), playerList, numOfPlayers);
            }
            else if (args == 2){
                //levelMoves = (int*) malloc((completedLevels+1) * sizeof(int)); // Memóriafoglalás
                player_AddPlayerToEnd(player_MakePlayer(name, completedLevels, statsListHead), playerList, numOfPlayers);
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

    // Ide még kell a rendezés név szerint
}

void player_WriteTxtFile(Player *playerListHead, int numOfPlayers){
    FILE *fp = fopen(playerDataPath, "wt");
    if (fp == NULL){
        perror("Nem sikerult megnyitni a player.txt fajlt.");
        return;
    }
    // irandó Sor létrehozása
    char printer[maxLineLenght];
    for(Player *mover = playerListHead; mover != NULL; mover = (Player *) mover->next){ // Listán végigjáró ciklus
        sprintf(printer,"%s;%d", mover->name, mover->numOfCompletedLevels);
        for (Statistics* stat = (Statistics *) mover->levelStats; stat != NULL; stat = (Statistics *) stat->next){
            sprintf(printer, "%s;%d", printer, stat->stepCount); // Sorhoz fűzés

        }
        fprintf(fp, "%s\n", printer); // Fájlba írás
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
        if ((*playerNode)->levelStats != NULL)
            FreeStatisticsList((Statistics **) &((*playerNode)->levelStats));
//            free((*playerNode)->levelStats);
        free(*playerNode);
    }
}

Player *player_MakePlayer(char name[], int numOfLevels, Statistics *statsListHead){
    // Memóriafoglalás
    Player *uj = (Player *) malloc(sizeof(Player));
    if (uj == NULL){
        perror("Nem sikerult az uj Player node-nak memoriat foglalni.");
        return NULL;
    }
    // Adatok átadása
    strcpy( uj->name, name); // Név
    uj->numOfCompletedLevels = numOfLevels; // Teljesített szintek Száma
    uj->levelStats = (struct Statistics *) statsListHead; // Lépések száma szintenként
    uj->next = NULL;
    return uj;
}

void player_AddPlayerToEnd(Player *newPlayer, Player **playerListHead, int *numOfPlayers){
    if (*playerListHead == NULL){ // Ha a lista üres, akkor a fejléc legyen az első elem
        *playerListHead = newPlayer;
    }
    else{ // Különben végiglépkedünk al istán amíg NULL-nem lesz a next pointer és oda illesztjük be az új elemet
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
    // removablePlayer keresése a név alapján
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
        char text[maxLineLenght];
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
    int indent = 8;// ->║ Név ║
    int line = 0, maxline = 0;
    int lenght = 0;
    int playerindex = 0;
    printfc("╔═══════╦═",p.x, p.y, baseForeColor);
    printfc("║ Szint ║ ",p.x, p.y+1, baseForeColor);
    printfc("╠═══════╬═ ",p.x, p.y+2, baseForeColor);
    char text[nameLenght+5], helper[nameLenght+5];
    while (mover != NULL){
        line = 2;
        lenght = (int) strlen(mover->name);
        // Fejléc kiiratása
        for(int i = 0; i < lenght + 2; i++) printfc("═╦", p.x + indent + i + 1, p.y, baseForeColor);
        sprintf(text, "║ %s ", mover->name); // Eltolás még eggyel nagypapa sornál
        printfc(text, p.x+indent, p.y+1, baseForeColor);
        for(int i = 0; i < lenght + 2; i++) printfc("═╬", p.x + indent + i + 1, p.y + 2, baseForeColor);
        // Szint számának kiiratása
        line++;
        int i = 0;
        for (Statistics* stat = (Statistics *) mover->levelStats; stat != NULL; stat = (Statistics *) stat->next){
            // Szint sorszámának kiírása
            sprintf(text, "║ %3d.  ║", i+1);
            printfc(text, p.x, p.y+i+3, baseForeColor);
            // Lépésszám kiratása
            strcpy(text, "║ %");
            sprintf(helper, "%d\0", lenght - 2);
            strcat(text, helper);
            strcat(text,"dl  ║\0");
            sprintf(helper, text, stat->stepCount);// Nagypapa sprnál hiba itt
            printfc(helper, p.x+indent, p.y+i+3, baseForeColor);
            line++;
            i++;
        }
        spaces[playerindex++] = lenght;
        indent += 3 + lenght;
        mover = (Player *) mover->next;
        maxline = line > maxline ? line : maxline;
    }

    printfc("╚═══════╩═",p.x,p.y+maxline,baseForeColor);
    indent = 9;
//    for(int s = 0; s < numOfPlayer; s++){
//    }
    for(int j = 0; j < numOfPlayer; j++){
        for(int i = 0; i < spaces[j] + 2; i++) printfc("═╩", p.x + indent + i, p.y + maxline, baseForeColor);
        for(int k = 3; k < maxline; k++) printfc("║", p.x+indent-1, p.y+k, baseForeColor);
        indent += spaces[j] + 3;
    }
    print("╗", p.x+indent-1, p.y);
    print("║", p.x+indent-1, p.y+1);
    print("╣", p.x+indent-1, p.y+2);
    for(int i = 3; i < maxline; i++) printfc("║", p.x+indent-1, p.y+i, baseForeColor);
    print("╝", p.x+indent-1, p.y+maxline);
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