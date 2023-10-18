#include "debugmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include "main.h"
#include "econio.h"
#include "datatypes.h"

#ifdef _WIN32
    #include <windows.h>
#endif

int const nameLenght = 20;


int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    // == Változók ==
    int totalWidth = 25,  // Szélesség
        totalHeight = 25; // Magasság
    Point playerPos; // A Játékos poziciója

    // Főképernyő kiiratása és Szint választás
    int selectedPlayer = 0;
    int selectedLevel = 0;
    Player *currentPlayer = NULL;
    char selectedLevelFileName[50];
    MainScreen(&currentPlayer, selectedLevelFileName);

    //char **map; // = (char*)malloc(totalHeight * totalWidth * sizeof(char)); // Ez lesz a dinamikus tömb ami a pályát tárolja
    //
    //ReadXSBFile(selectedLevelFileName, &map);

    //    Debugmalloc
    econio_clrscr();
    econio_gotoxy(0,0);
    econio_textbackground(COL_RESET);
    econio_textcolor(COL_RED);
    debugmalloc_dump();
     // Free up allocated memory
    FreePlayerList(&currentPlayer);
    // Debug after Free
    econio_textbackground(COL_RESET);
    econio_textcolor(COL_GREEN);
    debugmalloc_dump();
    scanf("%d", &selectedPlayer);
    return 0;
}
void MainScreen(Player **PlayerList, char *selectedLevelFileName){

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
    econio_gotoxy(16,20); printf("↑    ");
    econio_gotoxy(3,21); printf("Navigálás: ← ↓ →  ↲");

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
    char **levelFileNames; // String array
    enum State { exitApp, chosePlayer, choseLevel, startLvl};
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
                if (state == startLvl)  runMenu = false; //.... Pálya kiválasztása, játék indítása;
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
                if (state == chosePlayer && selectedPlayer > 0) selectedPlayer--;
                if (state == choseLevel && selectedLevel > 0) selectedLevel--;
                break;
            case KEY_DOWN:
                if (state == chosePlayer && selectedPlayer < numOfPlayers-1) selectedPlayer++;
                if (state == choseLevel && selectedLevel < numOfLevels-1) selectedLevel++;
                break;
        }
        #pragma endregion Lenyomot_ billentyű_kiértékelése

        #pragma region Képernyőre_írás_mód_szerint
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
                 _x = 12; _y = 9;
                if (displayFirst){
                    ClearScreenSection(0, 8, 60, 19, COL_RESET);
                    displayFirst = false;
                    econio_gotoxy(_x+10,_y-1);
                    econio_textcolor(COL_LIGHTCYAN);
                    printf("JÁTÉKOSOK:");
                    // Fájl beolvasása
                    ReadPlayerTxtFile(PlayerList, &numOfPlayers);
                }
                PrintPlayerList(*PlayerList, numOfPlayers,selectedPlayer,(Point) {_x,_y});
                    econio_gotoxy(0,18);
                    debugmalloc_dump();
                break;
            case choseLevel:
                _x = 12; _y = 9;
                if (displayFirst){
                    displayFirst = false;
                    ClearScreenSection(0, 8, 60, 19, COL_RESET);
                    econio_gotoxy(_x+10,_y-1);
                    econio_textcolor(COL_LIGHTCYAN);
                    printf("SZINTEK:");
                    econio_gotoxy(0,_y);
                    ReadDirectoryLevelNames("./levels/", &levelFileNames, &numOfLevels);
                }
                PrintLevels(levelFileNames,numOfLevels, selectedLevel,(*PlayerList)->completedLevels, (Point) {_x, _y});
                break;
            case startLvl:
                break;
        }
        #pragma endregion Képernyőre_írás_mód_szerint
        econio_gotoxy(0,0); // Kurzor bal felső sarokba
        if (runMenu) key = econio_getch(); // Billentyűlenyomásra vár, ha fut a menü
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
void FreePlayerList(Player **playerList){
    while (playerList != NULL) {
        Player *temp = (Player*) (*playerList)->next;
        free(playerList);
        *playerList = temp;
    }
}
void PrintPlayerList(Player *playerList, int numOfPlayers, int selectedPlayerIndex, Point start){
    int currentIndex = 0;
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
        printf("%s,  Szint: %d, Átlag: %d  lépés", playerList->name, playerList->completedLevels, playerList->averageMoves);
        playerList = (Player*) playerList->next;
        currentIndex++;
        econio_textbackground(COL_RESET);
    }
}
/*void PrintMap(char const **map, int width, int height){
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            printf("%c", map[y][x]);
        }
        printf("\n");
    }
}*/
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
void ReadDirectoryLevelNames(char directory[], char ***fileNames, int *numOfFiles){
    DIR *folder = opendir(directory);
    struct dirent *entry;
    *numOfFiles = 0;
    if(folder == NULL){
        printf("Nem lehet megnyitni a fájlt\n");
        perror("Unable to read directory");
    }
    while((entry=readdir(folder)))
    {
        if (entry->d_name[0] != '.'){
            numOfFiles++;
            //printf("File %3d: %s\n", files, entry->d_name);
        }
    }
    closedir(folder);
    folder = opendir(directory);
    int i = 0;
    fileNames = (char**) malloc(*numOfFiles * sizeof(char*));
    while((entry=readdir(folder)))
    {
//        printf("%d: %s\n",i, entry->d_name);
        if (entry->d_name[0] != '.'){ // Miért akad el a 78.-nál? CN4D-...
            //fileNames[i] = (char*) malloc(entry->d_namlen);
            fileNames[i] = entry->d_name;
            i++;
        }
    }
    closedir(folder);
}
void PrintLevels(char *levelList[], int numOfLevels, int selectedLevel, int maxLevels, Point start){
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
void ReadXSBFile(char const filename[], char ***map){
//    char **newmap;
//    FILE* fp = fopen(filename, "r");
// foglalás
//    newmap = (char**) malloc(magassag * sizeof(char*));
//    for (int y = 0; y < magassag; ++y)
//        szamok[y] = (char*) malloc(szelesseg * sizeof(char));

}
