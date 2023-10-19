#include "debugmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include "main.h"
#include "econio.h"
#include "datatypes.h"
#include "player.h"
#include "game.h"
#ifdef _WIN32
    #include <windows.h>
#endif


int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    // == Változók ==
    Point playerPos; // A Játékos poziciója

    // Főképernyő kiiratása és Szint választás
    int selectedPlayer = 0;
    int selectedLevel = 0;
    Player *currentPlayer = NULL;
    char selectedLevelFileName[50];
    // Főképernyő meghívása
    MainScreen(&currentPlayer, selectedLevelFileName);
    // Valamiért nem adja kivela a currentPlayer az értékét.
    printf("Játékos: %s, Szint: %s\n", currentPlayer->name, selectedLevelFileName);
    StartGame(currentPlayer, selectedLevelFileName);
    return 0;
}



void MainScreen(Player **currentPlayer, char selectedLevelFileName[]){

    #pragma region FŐCÍM_KIÍRÁSA
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
    econio_gotoxy(16,21); printf("↑    ⌫");
    econio_gotoxy(3,22); printf("Navigálás: ← ↓ →  ↲");

    #pragma endregion FŐCÍM_KIÍRÁSA

    // Menü Változók
    int key = -1;
    bool displayFirst = true;
    bool runMenu = true;
    int option = 0;
    int numOfPlayers = 0;
    int numOfLevels = 0;
    int selectedPlayer = 0;
    int selectedLevel = 0;
    char **levelFileNames = NULL; // String array
    Player *PlayerList = NULL;
    char newPlayerName[21];
    enum State { exitApp, chosePlayer, choseLevel, startLvl, addedNewPlayer};
    enum State state = chosePlayer;

    econio_rawmode();
    // Nehézség kiválasztása
    while(runMenu){

        #pragma region Lenyomot_ billentyű_kiértékelése
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
                //if (state == startLvl)  runMenu = false; //.... Pálya kiválasztása, játék indítása;
                displayFirst = true;
                option = 0;
                break;
            case KEY_RIGHT:
                if (state == exitApp && option == 0) option = 1;
                break;
            case KEY_LEFT:
                if (state == exitApp && option == 1) option = 0;
                break;
            case KEY_UP:
                if (state == chosePlayer && selectedPlayer > 0){
                    selectedPlayer--;
                }
                if (state == choseLevel && selectedLevel > 0) selectedLevel--;
                break;
            case KEY_DOWN:
                if (state == chosePlayer && selectedPlayer < numOfPlayers-1){
                    selectedPlayer++;
                }
                if (state == choseLevel && selectedLevel < (*currentPlayer)->completedLevels-1) selectedLevel++;
                break;
        }
        #pragma endregion Lenyomot_ billentyű_kiértékelése

        #pragma region Képernyőre_írás_mód_szerint
        int _x, _y;
        switch (state) {
            case exitApp:
                _x = 25, _y = 8;
                if (displayFirst) {
                    displayFirst = false;
                    ClearScrBellow();
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
                _x = 12; _y = 9;
                if (displayFirst){
                    // Kiiratás inicializálása
                    displayFirst = false;
                    player_FreePlayerList(&PlayerList);
                    ClearScrBellow();
                    econio_gotoxy(_x+20,_y-1);
                    econio_textcolor(COL_LIGHTCYAN);
                    // Alcím kiírása
                    printf("JÁTÉKOSOK:");
                    // Fájl beolvasása
                    player_ReadTxtFile(&PlayerList, &numOfPlayers);
                }
                player_PrintPlayerList(PlayerList, numOfPlayers, selectedPlayer, (Point) {_x, _y});
                break;
            case choseLevel:
                _x = 22; _y = 9;
                if (selectedPlayer == 0){
                    if (displayFirst){
                        // Kiiratás inicializálása
                        displayFirst = false;
                        ClearScrBellow();
                        econio_gotoxy(_x+6,_y-1);
                        econio_textcolor(COL_LIGHTCYAN);
                        // Alcím kiírása
                        printf("Név (max 20 karakter):");
                        econio_gotoxy(_x+7,_y);
                        printf("____________________");
                        econio_gotoxy(_x+7,_y);
                        econio_normalmode();
                        if (scanf("%20s[^\n]", newPlayerName) == 1){
                            runMenu = false;
                        }
                        // Még átdolgozást igényel!!!
                        econio_rawmode();
                    }
                }
                else {
                    if (displayFirst){
                        // Kiiratás inicializálása
                        displayFirst = false;
                        ClearScrBellow();
                        FreeLevelList(&levelFileNames,numOfLevels);
                        econio_gotoxy(_x+10,_y-1);
                        econio_textcolor(COL_LIGHTCYAN);
                        // Alcím kiírása
                        printf("SZINTEK:");
                        //econio_gotoxy(0,_y);
                        // Szintek beolvasása
                        ReadDirectoryLevelNames("./levels/", &levelFileNames, &numOfLevels);
                        // currentPlayer Kiválasztása a láncolt Listából selectedPlayer alapján
                        *currentPlayer = player_GetSelectedPlayer(PlayerList, selectedPlayer);
                        selectedLevel = (*currentPlayer)->completedLevels-1;
                    }
                    PrintLevels(levelFileNames, numOfLevels, selectedLevel, (*currentPlayer)->completedLevels, (Point) {_x+9, _y});
                }
                break;
            case startLvl:
                runMenu = false;
                strcpy(selectedLevelFileName, levelFileNames[selectedLevel]);
                break;
        }
        #pragma endregion Képernyőre_írás_mód_szerint
        if (!(state == choseLevel && selectedPlayer ==0) ) econio_gotoxy(0,0); // Kurzor bal felső sarokba
        if (runMenu) key = econio_getch(); // Billentyűlenyomásra vár, ha fut a menü
    }

    debugmalloc_log_file("MainScreenEndBeforeFree.txt");
    // Free dinamikus tömbök
    FreeLevelList(&levelFileNames,numOfLevels);
    player_FreePlayerList(&PlayerList);

    // Utána:
    debugmalloc_log_file("MainScreenEndAfterFree.txt");
    econio_normalmode();
}
void ClearScrBellow(){
    ClearScreenSection(0, 8, 70, 20, COL_RESET);
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

void ReadDirectoryLevelNames(char directory[], char **levelList[], int *numOfFiles){
    DIR *folder = opendir(directory);
    struct dirent *dir;
    int count = 0;
    if(folder == NULL){
        printf("Nem lehet megnyitni a fájlt\n");
        perror("Unable to read directory");
    }
    while((dir=readdir(folder)))
    {
        if (dir->d_name[0] != '.'){
            count++;
            //printf("File %3d: %s\n", files, dir->d_name);
        }
    }
    rewinddir(folder);
    int i = 0;
    char **fileList = (char**) malloc(count * sizeof(char*));
    while((dir=readdir(folder)))
    {
        if (dir->d_name[0] != '.'){
            //printf("%d: %s\n",i, dir->d_name);
            fileList[i] = (char*) malloc(strlen(dir->d_name)+1);
            strcpy(fileList[i],dir->d_name);
            i++;
        }
    }
    *levelList = fileList;
    *numOfFiles = count;
    closedir(folder);
}
void FreeLevelList(char ***levelList, int numOfLevels){
    if (*levelList != NULL){
        for(int i = 0; i < numOfLevels; i++)
        {
            free((*levelList)[i]);
        }
        free(*levelList);
    }
}
void PrintLevels(char **levelList, int numOfLevels, int selectedLevel, int maxLevels, Point start){
    int currentIndex = 0;
    while(currentIndex < maxLevels && currentIndex < numOfLevels) {
        econio_gotoxy(start.x, start.y + currentIndex);
        if (currentIndex == selectedLevel){
            econio_textcolor(COL_BLUE);
            econio_textbackground(COL_LIGHTCYAN);
        }
        else {
            econio_textcolor(COL_LIGHTCYAN);
            econio_textbackground(COL_RESET);
        }
        printf("%s", levelList[currentIndex]);
        currentIndex++;
        econio_textbackground(COL_RESET);
    }
}


