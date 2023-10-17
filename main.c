#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include "main.h"
#include "econio.h"
#include "datatypes.h"
#include "datapath.h"

#ifdef _WIN32
    #include <windows.h>
#endif

/* == Konstansok == */

char const chrPlayer[] = "◉", //◎,
           chrPlyerOnTarget[] = "◉",
           chrWall[] = "█",
           chrTarget[] = "◈",
           chrBox[] = "▢", //◾"//"◽""⊠",
           chrBoxOnTarget[] = "▣";
int const colorPlayer = COL_CYAN,
          colorPlayerOnTarget = COL_BLUE,
          colorBox = COL_BROWN,
          colorBoxOnTarget = COL_LIGHTGREEN,
          colorWall = COL_LIGHTGRAY,
          colorTarget = COL_RED,
          maxDificulty = 16,
          minDificulty = 1,
          nameLenght = 20;


int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    // == Változók ==
    int totalWidth = 25,  // Szélesség
        totalHeight = 25; // Magasság
    Point playerPos; // A Játékos poziciója

    // Főképernyő kiiratása és Szint választás
    int selectedPlayer = 0,
        selectedLevel = 1;
    Player *currentPlayer = NULL;
    MainScreen(&selectedPlayer, &selectedLevel, &currentPlayer);

    char **map; // = (char*)malloc(totalHeight * totalWidth * sizeof(char)); // Ez lesz a dinamikus tömb ami a pályát tárolja
    //ReadXSBFile(selectedLevelFileName, &map);


     // Free up allocated memory
    FreePlayerList(currentPlayer);
    return 0;
}
void MainScreen(int *selectedPlayer, int *selectedLevel, Player **PlayerList){
    // CÍM Kiírása
    econio_textcolor(COL_LIGHTBLUE);
    printf("▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n");
    econio_textcolor(COL_CYAN);
    printf("       ███████  ██████  ██   ██  ██████  ██████   █████  ███    ██ \n"
           "       ██      ██    ██ ██  ██  ██    ██ ██   ██ ██   ██ ████   ██ \n"
           "       ███████ ██    ██ █████   ██    ██ ██████  ███████ ██ ██  ██ \n"
           "            ██ ██    ██ ██  ██  ██    ██ ██   ██ ██   ██ ██  ██ ██ \n"
           "       ███████  ██████  ██   ██  ██████  ██████  ██   ██ ██   ████ \n");
    econio_textcolor(COL_LIGHTBLUE);
    printf("▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n");
    //econio_textbackground(COL_RESET);

    econio_textcolor(COL_LIGHTCYAN);
    econio_gotoxy(16,20); printf("↑    ");
    econio_gotoxy(3,21); printf("Navigálás: ← ↓ →  ↲");

    // Menü Változók
    int key = -1;
    bool displayFirst = true;
    bool runMenu = true;
    int option = 0;
    int numOfPlayers = 0; // !!!!!!!!!!!!!!!!!!!!!!
    enum State { exitApp, chosePlayer, choseLevel, startLvl};
    enum State state = chosePlayer;

    econio_rawmode();
    // Nehézség kiválasztása
    while(runMenu){
        // Lenyomott billentyű kiértékelése
        switch (key){
            case KEY_ESCAPE:
            case KEY_BACKSPACE:
                if (state > 0) state--; // Módváltás
                displayFirst = true;
                option = 0;
                break;
            case KEY_ENTER:
                if (state == exitApp && option == 0) { runMenu = false; }
                if (state < startLvl) state++; // Módváltás
                displayFirst = true;
                option = 0;
                *PlayerList = NULL;
                break;
            case KEY_RIGHT:
                if (state == exitApp && option == 0) option = 1;
                break;
            case KEY_LEFT:
                if (state == exitApp && option == 1) option = 0;
                break;
            case KEY_UP:
                if (state == chosePlayer && *selectedPlayer > 0) (*selectedPlayer)--;
                break;
            case KEY_DOWN:
                if (state == chosePlayer && *selectedPlayer < numOfPlayers-1) (*selectedPlayer)++;
                break;
        }
        // Képernyőre írás a mód szerint
        char _x, _y;
        switch (state) {
            case exitApp:
                _x = 25, _y = 8;
                if (displayFirst) {
                    displayFirst = false;
                    ClearScreenSection(0, 8, 60, 19, COL_RESET);
                    econio_textcolor(COL_RED);
                    econio_gotoxy(_x, _y+1);
                    printf("╔════════════════════════╗\n");
                    econio_gotoxy(_x, _y+2);
                    printf("║    BIZTOSAN KILÉPSZ?   ║\n");
                    econio_gotoxy(_x, _y+3);
                    printf("║                        ║\n");
                    econio_gotoxy(_x, _y+4);
                    printf("║    IGEN        NEM     ║\n");
                    econio_gotoxy(_x, _y+5);
                    printf("╚════════════════════════╝\n");
                }
                if (option == 0) {
                    econio_gotoxy(_x+5, _y+4);
                    econio_textcolor(COL_WHITE); econio_textbackground(COL_LIGHTRED);
                    printf("IGEN");
                    econio_gotoxy(_x+17, _y+4);
                    econio_textcolor(COL_RED); econio_textbackground(COL_RESET);
                    printf("NEM");
                }
                else {
                    econio_gotoxy(_x+5, _y+4);
                    econio_textcolor(COL_RED);
                    econio_textbackground(COL_RESET);
                    printf("IGEN");
                    econio_gotoxy(_x+17, _y+4);
                    econio_textcolor(COL_WHITE);
                    econio_textbackground(COL_LIGHTRED);
                    printf("NEM");
                }
                break;
            case chosePlayer:
                 _x = 12;
                if (displayFirst){
                    ClearScreenSection(0, 8, 60, 19, COL_RESET);
                    displayFirst = false;
                    econio_gotoxy(_x+10,8);
                    econio_textcolor(COL_LIGHTCYAN);
                    printf("JÁTÉKOSOK:\n");
                    // Fájl beolvasása
                    ReadPlayerTxtFile(PlayerList, &numOfPlayers);
                }
                PrintPlayerList(*PlayerList, numOfPlayers,*selectedPlayer,(Point) {_x,_y});

                break;
            case choseLevel:
                ClearScreenSection(0, 8, 60, 19, COL_RESET);
                break;
            case startLvl:
                break;
        }
        econio_gotoxy(0,0);
        if (runMenu) key = econio_getch();
    }

    econio_normalmode();
    ClearScreenSection(0, 8, 60, 19, COL_RESET);
    printf("KILÉP: %d\n", state);

    /*econio_gotoxy(12,12);
    printf("SZINT: ");
    key = -1;

    char hex[16] = "0123456789ABCDEF";
    // Szintek beolvasása
    DIR *d = opendir("./levels/");  // melyik mappa
    if (d == NULL) { perror("Nem sikerült megnyitni"); }
    else {
        struct dirent *de;
        while ((de = readdir(d)) != NULL) {
            if (de->d_name[0] == hex[*selectedPlayer]){
                printf("%s\n", de->d_name);
            }
        }
        closedir(d);
    }*/

//    econio_gotoxy(12,10);
//    printf("Selected: %d", *selectedPlayer);
}
void ReadPlayerTxtFile(Player **playerList, int *numOfPlayers) {
    /*DIR *folder;
    struct dirent *entry;
    int files = 0;

    folder = opendir(".");
    if(folder == NULL)
    {
        perror("Unable to read directory");
    }

    while( (entry=readdir(folder)) )
    {
        files++;
        printf("File %3d: %s\n",
               files,
               entry->d_name
        );
    }

    closedir(folder);*/
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
            if (sscanf(inputLine, "%20[^;];%d;%d;%d", name, &completedLevels,&totalMoves,&averageMoves) == 4){
                AddPlayer(MakePlayer(name, completedLevels, totalMoves, averageMoves), playerList, numOfPlayers);
            }
        }
    }
    fclose(fp);
}
void AddPlayer(Player *newPlayer, Player **playerList, int *numOfPlayers){
    if (*playerList != NULL) (*playerList)->back = (struct Player*) newPlayer;
    newPlayer->next = (struct Player*) *playerList;
    newPlayer->back = NULL;
    *playerList = newPlayer;
    (*numOfPlayers)++;
}
Player *MakePlayer(char name[], int completedLevels, int totalMoves, int averageMoves){
    Player *uj = (Player *) malloc(sizeof(Player));
    strcpy(uj->name, name);
    uj->completedLevels = completedLevels;
    uj->totalMoves = totalMoves;
    uj->averageMoves = averageMoves;
    uj->next = NULL;
    uj->back = NULL;
    return uj;
}
void FreePlayerList(Player *playerList){
    while (playerList != NULL) {
        Player *temp = (Player*) playerList->next;
        free(playerList);
        playerList = temp;
    }
}
void PrintPlayerList(Player *playerList, int numOfPlayers, int selectedPlayerIndex, Point start){
    int currentIndex = 0;
    while(playerList != NULL){
        // kiirás !!!!!!!!!!!!!!!!!!
        econio_gotoxy(start.x, 9 + currentIndex);
        if (currentIndex == selectedPlayerIndex){
            econio_textcolor(COL_BLUE);
            econio_textbackground(COL_LIGHTCYAN);
        }
        else {
            econio_textcolor(COL_LIGHTCYAN);
            econio_textbackground(COL_RESET);
        }
        printf("%s,  Szint: %d, Átlag: %d  lépés", playerList->name, playerList->completedLevels, playerList->averageMoves);
        playerList = (Player*) playerList->next;
        currentIndex++;
        econio_textbackground(COL_RESET);
    }
}
void PrintMap(char const **map, int width, int height){
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            printf("%c", map[y][x]);
        }
        printf("\n");
    }
}
void ClearScreenSection(int x1, int y1, int x2, int y2, EconioColor bgColor){
    econio_gotoxy(x1,y1);
    econio_textcolor(bgColor);
    econio_textbackground(COL_RESET);
    for (int i = 0; i < y2-y1; i++){
        for(int j = 0; j < x2-x1; j++){
            printf(" ");
        }
        econio_gotoxy(x1,y1+i);
    }
}
void ReadXSBFile(char const filename[], char ***map){
//    char **newmap;
//    FILE* fp = fopen(filename, "r");
// foglalás
//    newmap = (char**) malloc(magassag * sizeof(char*));
//    for (int y = 0; y < magassag; ++y)
//        szamok[y] = (char*) malloc(szelesseg * sizeof(char));

}
