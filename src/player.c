#include <stdio.h>
#include <string.h>
#include "../libraries/debugmalloc.h"
#include "../libraries/econio.h"
#include "../headers/datatypes.h"
#include "../headers/player.h"
#include "../headers/lib.h"
#include "../headers/statistics.h"



void player_ReadTxtFile(Player **playerListHead, int *numOfPlayers) {
    *numOfPlayers = 0;
    FILE *fp;
    fp = fopen(playerDataPath, "r");
    if (fp == NULL) {
        lib_printError("Nem lehet megnyitni a player.txt fájlt az olvasáshoz!");
        return;
    }

    player_FreePlayerList(playerListHead); // Ha van lefoglalva már, akkor felszabadítja a players listát
        char inputLine[maxLineLenght], name[nameLenght*2+1], inputRest[maxLineLenght];
        int completedLevels;
        int stepCount;
        int i;
        while (fgets(inputLine, maxLineLenght, fp)) {
            Statistics *statsListHead = NULL;
            // %20 helyére a (int const) nameLenght értékét mindig
            int args = sscanf(inputLine, "%40[^;];%d;%s[^\n]", name, &completedLevels, inputRest);
            if (args == 3){
                int actualNumOfCompletedLevels = completedLevels;
                for(i = 0; i < completedLevels-1; i++){
                    if (sscanf(inputRest, "%d;%s[^\n]", &stepCount, inputRest) == 2)
                        stats_AddLevelStatistics(stepCount, &statsListHead);
                    else
                        actualNumOfCompletedLevels--;
                }
                sscanf(inputRest, "%d", &stepCount);
                stats_AddLevelStatistics(stepCount, &statsListHead);
                completedLevels = actualNumOfCompletedLevels;
                player_AddPlayerInOrder(player_MakePlayer(name, completedLevels, statsListHead), playerListHead, numOfPlayers);
            }
            else if (args == 2){
                player_AddPlayerInOrder(player_MakePlayer(name, completedLevels, statsListHead), playerListHead, numOfPlayers);
            }
            else if (*playerListHead == NULL) {
                return;
            }
            else{
                lib_printError("Hiba a player.txt beolvasásánál, nem megfelelő bemenet!");
            }
        }
    fclose(fp); // Fájl bezárása
}

void player_WriteTxtFile(Player *playerListHead){
    FILE *fp = fopen(playerDataPath, "wt");
    if (fp == NULL){
        lib_printError("Nem sikerült megnyitni a player.txt fájlt az íráshoz!");
        return;
    }
    // irandó Sor létrehozása
    char printer[maxLineLenght-1];
    for(Player *mover = playerListHead; mover != NULL; mover = (Player *) mover->next){ // Listán végigjáró ciklus
        sprintf(printer,"%s;%d", mover->name, mover->numOfCompletedLevels);
        char helper[maxLineLenght+1];
        for (Statistics* stat = (Statistics *) mover->levelStats; stat != NULL; stat = (Statistics *) stat->next){
            sprintf(printer, "%s;%d", printer, stat->stepCount); // Sorhoz fűzés
//            strcat(printer, helper);
            // TODO: Itt hibát dob játékos törlésekor valmiért
        }
        fprintf(fp, "%s\n", printer); // Fájlba írás
    }
    fclose(fp);
}

void player_FreePlayerList(Player **playerListHead){
    Player *temp;
    while (*playerListHead != NULL) {
        temp = (Player*) (*playerListHead)->next;
        player_FreePlayerNode(playerListHead);
        *playerListHead = temp;
    }
}
 void player_FreePlayerNode(Player **playerNode){
    if (*playerNode != NULL){
        if ((*playerNode)->levelStats != NULL)
            stats_FreeStatisticsList((Statistics **) &((*playerNode)->levelStats));
        free(*playerNode);
    }
}

Player *player_MakePlayer(char name[], int numOfLevels, Statistics *statsListHead){
    // Memóriafoglalás
    Player *uj = (Player *) malloc(sizeof(Player));
    if (uj == NULL){
        lib_printError("Nem sikerült az új Player node-nak memóriat foglaln!");
        return NULL;
    }
    // Adatok átadása
    strcpy( uj->name, name); // Név
    uj->numOfCompletedLevels = numOfLevels; // Teljesített szintek Száma
    uj->levelStats = (struct Statistics *) statsListHead; // Lépések száma szintenként
    uj->next = NULL;
    return uj;
}

/*void player_AddPlayerToEnd(Player *newPlayer, Player **playerListHead, int *numOfPlayers){
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
}*/
void player_AddPlayerInOrder(Player *newPlayer, Player **playerListHead, int *numOfPlayers){
    Player *prev = NULL;
    Player *mover = *playerListHead;
    // Hely keresése
    while (mover != NULL && (stringlenght(newPlayer->name) > stringlenght(mover->name) || newPlayer->numOfCompletedLevels > mover->numOfCompletedLevels)) { // Névhossz szerint rendezés
        prev = mover;
        mover = (Player *) mover->next;
    }
    // Beszúrás
    if (prev == NULL){
        newPlayer->next = (struct Player *) *playerListHead;
        *playerListHead = newPlayer;
    }
    else{
        prev->next = (struct Player *) newPlayer;
        newPlayer->next = (struct Player *) mover;
    }
    (*numOfPlayers)++;
}
bool player_RemovePlayer(Player *removablePlayer, Player **playerListHead, int *numOfPlayers){
    // A Lista elejének eltárolása
    Player *temp = *playerListHead, *prev;
    // Player Keresése
    // Ha az első elem a keresett
    if (temp != NULL && strcmp(temp->name, removablePlayer->name) == 0) {
        *playerListHead = (Player *) temp->next; // Első elem továbbléptetése
        player_FreePlayerNode(&temp); // Törlés
        (*numOfPlayers)--;
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
        (*numOfPlayers)--;
        return true;
    }
}

void player_PrintPlayerList(Player *playerList, int selectedPlayerIndex, Point p){
    int currentIndex = 0;
    //lib_ClearScreenSection(0, 8, 60, 19, COL_RESET);
    if (playerList == NULL){
        printfc("Nincs egy játékos sem még.", p.x - (int)stringlenght("Nincs egy játékos sem még.") / 2, p.y + currentIndex, baseForeColor );
    }
    while(playerList != NULL){
        char text[maxLineLenght];
        sprintf(text,"%s, Szint: %d", playerList->name, playerList->numOfCompletedLevels);
        if (currentIndex == selectedPlayerIndex)
            printfbc(text, p.x - (int)stringlenght(text) / 2, p.y + currentIndex, activeForeColor, activeBgColor);
        else
            printfc(text, p.x - (int)stringlenght(text) / 2, p.y + currentIndex, baseForeColor);
        playerList = (Player*) playerList->next;
        currentIndex++;
    }

}

Player* player_GetSelectedPlayer(Player *playerListHead, int selectedPlayer){
    if (playerListHead != NULL){
        for (int i = 0; i < selectedPlayer; i++){
            playerListHead = (Player*) playerListHead->next;
        }
        return playerListHead;
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
