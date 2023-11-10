#include <stdio.h>
#include <string.h>
#include <string.h>
#include "../libraries/debugmalloc.h"
#include "../libraries/econio.h"
#include "../headers/player.h"
#include "../headers/statistics.h"
#include "../headers/datatypes.h"



void player_ReadTxtFile(Player **playerListHead, int *numOfPlayers) {
    *numOfPlayers = 0;
    FILE *fp;
    fp = fopen(playerDataPath, "r");
    if (fp == NULL) {
        printf("Nem lehet megnyitni a fájlt\n");
        return;
    }

    player_FreePlayerList(playerListHead); // Ha van lefoglalva már, akkor felszabadítja a players listát
        char inputLine[maxLineLenght], name[nameLenght*2], inputRest[maxLineLenght];
        int completedLevels;
        int stepCount;
        int i;
        while (fgets(inputLine, maxLineLenght, fp)) {
            Statistics *statsListHead = NULL;
            // %20 helyére a (int const) nameLenght értékét mindig
            int args = sscanf(inputLine, "%20[^;];%d;%s[^\n]", name, &completedLevels, inputRest);
            if (args == 3){
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
                //player_AddPlayerToEnd(player_MakePlayer(name, completedLevels, statsListHead), playerListHead, numOfPlayers);
                player_AddPlayerInOrder(player_MakePlayer(name, completedLevels, statsListHead), playerListHead, numOfPlayers);
            }
            else if (args == 2){
                //player_AddPlayerToEnd(player_MakePlayer(name, completedLevels, statsListHead), playerListHead, numOfPlayers);
                player_AddPlayerInOrder(player_MakePlayer(name, completedLevels, statsListHead), playerListHead, numOfPlayers);
            }
            else if (*playerListHead == NULL) {
                return;
            }
            else{
                perror("Hiba a player.txt beolvasasanal, nem megfelelo bemenet!");
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
void player_AddPlayerInOrder(Player *newPlayer, Player **playerListHead, int *numOfPlayers){
    Player *prev = NULL;
    Player *mover = *playerListHead;
    // Hely keresése
//    while (mover != NULL && strcmp(newPlayer->num, mover->name) > 0){ // ABCabccÁÉáé szerint rendetls
//    while (mover != NULL && newPlayer->numOfCompletedLevels > mover->numOfCompletedLevels){ // Szint szerint rendezés
    while (mover != NULL && stringlenght(newPlayer->name) > stringlenght(mover->name)){ // Névhossz szerint rendezés
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
        printfc("Nincs egy játékos sem még.", center.x - (int)stringlenght("Nincs egy játékos sem még.")/2,center.y + currentIndex, baseForeColor );
    }
    while(playerList != NULL){
        char text[maxLineLenght];
        sprintf(text,"%s, Szint: %d", playerList->name, playerList->numOfCompletedLevels);
        if (currentIndex == selectedPlayerIndex)
            printfbc(text,center.x - (int)stringlenght(text)/2,center.y + currentIndex, activeForeColor, activeBgColor);
        else
            printfc(text,center.x - (int)stringlenght(text)/2,center.y + currentIndex, baseForeColor);
        playerList = (Player*) playerList->next;
        currentIndex++;
    }

}
void PrintRankList(Player *playerList, int numOfPlayer, Point p, int maxWidth, int maxheight){
    // Segéd változók
    Player *mover = playerList;
    int *spaces = (int*) malloc(numOfPlayer * sizeof(int));
    int lenght = 0;
    int firstindent = 7; // Szint->║ Név ║
    int totalLenght = firstindent;
    int playerindex = 0;
    // Nevek hosszának megállapítása
    while (mover != NULL){
        lenght = (int) stringlenght(mover->name);
        totalLenght += lenght + 3;
        spaces[playerindex++] = lenght;
        mover = (Player *) mover->next;
    }
    // Segédváltozók visszaállítása
    p = (Point) {p.x - totalLenght/2, p.y};
    mover = playerList;
    playerindex = 0;
    int line = 0, maxline = 0;
    printfc(" Szint ┃ ",p.x, p.y + line++, baseForeColor);
    printfc("━━━━━━━╋ ",p.x, p.y + line++, baseForeColor);
    char text[nameLenght+5],
         helper[nameLenght+5];
    int indent = firstindent + 1;

    // Player listán végigmegyünk
    while (mover != NULL){
        line = 0;
        // Fejléc kiiratása
        sprintf(text, "┃ %s ", mover->name);
        printfc(text, p.x+indent-1, p.y+line++, baseForeColor);
            for(int i = 0; i < spaces[playerindex] + 2; i++) printfc("━", p.x + indent + i, p.y + line, baseForeColor);
            if (playerindex < numOfPlayer-1) printfc("╋", p.x + indent + + spaces[playerindex] + 2, p.y + line, baseForeColor);
        line++;
        // Szint számának kiiratása
        int lvl = 1;
        for (Statistics* stat = (Statistics *) mover->levelStats; stat != NULL; stat = (Statistics *) stat->next){
            // Szint sorszámának kiírása
            sprintf(text, "  %3d. ┃", lvl);
            printfc(text, p.x, p.y+line, baseForeColor);
            // Lépésszám kiratása
            strcpy(text, "%");
            sprintf(helper, "%d\0", spaces[playerindex]);
            strcat(text, helper);
            strcat(text,"dl\0");
            sprintf(helper, text, stat->stepCount);// Nagypapa sprnál hiba itt
            printfc(helper, p.x+indent, p.y+line, baseForeColor);
            line++;
            lvl++;
        }
        mover = (Player *) mover->next;
        indent += 3 + spaces[playerindex++];
        maxline = line > maxline ? line : maxline;
    }

    indent = firstindent + 1;
    for(int j = 0; j < numOfPlayer-1; j++){
        indent += spaces[j] + 3;
        for(int k = 2; k < maxline; k++) printfc("┃", p.x+indent-1, p.y+k, baseForeColor);
    }
//    for(int i = 2; i < maxline; i++) printfc("│", p.x+indent-1, p.y+i, baseForeColor);
    free(spaces);
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