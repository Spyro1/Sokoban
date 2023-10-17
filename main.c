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
          minDificulty = 1;


int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    // == Változók ==
    int totalWidth = 25,  // Szélesség
        totalHeight = 25; // Magasság
    Point playerPos; // A Játékos poziciója

    // Főképernyő kiiratása és Szint választás
    int selectedDificulty = 1,
        selectedLevel = 1;
    Player *currentPlayer = NULL;
    MainScreen(&selectedDificulty, &selectedLevel, &currentPlayer);

    char **map; // = (char*)malloc(totalHeight * totalWidth * sizeof(char)); // Ez lesz a dinamikus tömb ami a pályát tárolja
    //ReadXSBFile(selectedLevelFileName, &map);


     // Free up allocated memory
    FreePlayerList(currentPlayer);
    return 0;
}
void MainScreen(int *selectedPlayer, int *selectedLevel, Player **PlayerList){
    // CÍM Kiírása
    /*printf("\n"
           "#   _______  _______  ___   _  _______  _______  _______  __    _ \n"
           "#  |       ||       ||   | | ||       ||  _    ||   _   ||  |  | |\n"
           "#  |  _____||   _   ||   |_| ||   _   || |_|   ||  |_|  ||   |_| |\n"
           "#  | |_____ |  | |  ||      _||  | |  ||       ||       ||       |\n"
           "#  |_____  ||  |_|  ||     |_ |  |_|  ||  _   | |       ||  _    |\n"
           "#   _____| ||       ||    _  ||       || |_|   ||   _   || | |   |\n"
           "#  |_______||_______||___| |_||_______||_______||__| |__||_|  |__|");*/
    /*printf("\n"
           " ________  ________  ___  __    ________  ________  ________  ________      \n"
           "|\\   ____\\|\\   __  \\|\\  \\|\\  \\ |\\   __  \\|\\   __  \\|\\   __  \\|\\   ___  \\    \n"
           "\\ \\  \\___|\\ \\  \\|\\  \\ \\  \\/  /|\\ \\  \\|\\  \\ \\  \\|\\ /\\ \\  \\|\\  \\ \\  \\\\ \\  \\   \n"
           " \\ \\_____  \\ \\  \\\\\\  \\ \\   ___  \\ \\  \\\\\\  \\ \\   __  \\ \\   __  \\ \\  \\\\ \\  \\  \n"
           "  \\|____|\\  \\ \\  \\\\\\  \\ \\  \\\\ \\  \\ \\  \\\\\\  \\ \\  \\|\\  \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \n"
           "    ____\\_\\  \\ \\_______\\ \\__\\\\ \\__\\ \\_______\\ \\_______\\ \\__\\ \\__\\ \\__\\\\ \\__\\\n"
           "   |\\_________\\|_______|\\|__| \\|__|\\|_______|\\|_______|\\|__|\\|__|\\|__| \\|__|\n"
           "   \\|_________|\n\n\n");*/
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

    econio_textcolor(COL_LIGHTCYAN);
    //econio_textbackground(COL_RESET);

    econio_gotoxy(16,20); printf("↑    ");
    econio_gotoxy(3,21); printf("Navigálás: ← ↓ →  ↲");

    // Menü Változók
    int key = -1;
    bool displayFirst = true;
    bool runMenu = true;
    int option = 0;
    int numOfPlayers = 0; // !!!!!!!!!!!!!!!!!!!!!!
    enum State { exit, chosePlayer, choseLevel, startLvl};
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
                if (state == exit && option == 0) { runMenu = false; }
                if (state < startLvl) state++; // Módváltás
                displayFirst = true;
                option = 0;
                break;
            case KEY_RIGHT:
                //if (state == choseDificulty && *selectedPlayer < maxDificulty) (*selectedPlayer)++;
                if (state == exit && option == 0) option = 1;
                break;
            case KEY_LEFT:
                //if (state == choseDificulty && *selectedPlayer > minDificulty) (*selectedPlayer)--;
                if (state == exit && option == 1) option = 0;
                break;
            case KEY_UP:
                if (state == chosePlayer && *selectedPlayer > 0) (*selectedPlayer)--;
                break;
            case KEY_DOWN:
                if (state == chosePlayer && *selectedPlayer < numOfPlayers-1) (*selectedPlayer)++;
                break;
        }
        // Képernyőre írás a múd szerint
        char _x, _y;
        switch (state) {
            case exit:
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
                    printf("JÁTÉKOSOK:\n");
                    // Fájl beolvasása
                    ReadPlayerTxtFile(PlayerList);
                }
                int item = 0;
                while((*PlayerList) != NULL){
                    // kiirás
                    econio_gotoxy(_x,9+item);
                    if (item == option){
                        econio_textcolor(COL_DARKGRAY);
                        econio_textbackground(COL_LIGHTBLUE);
                    }
                    else {
                        econio_textcolor(COL_CYAN);
                        econio_textbackground(COL_RESET);
                    }
                    printf("%s Teljesített szintek:  %d", (*PlayerList)->name, (*PlayerList)->completedLevels);
                    (*PlayerList) = (*PlayerList)->next;
                    item++;
                    econio_textbackground(COL_RESET);
                }
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
void ReadPlayerTxtFile(Player **players) {
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
    FILE *fp;
    fp = fopen("./players.txt", "r");
    if (fp == NULL) { // ???? Miért nem nyitja meg a fájlt ????
        printf("Nem lehet megnyitni a fájlt\n");
    } else {
        char inputLine[100];
        char name[50];
        int completedLevels, totalMoves, averageMoves;

        while (fgets(inputLine, 100, fp)) {
            sscanf(inputLine, "%50[^;];%d;%d;%d", name, &completedLevels,&totalMoves,&averageMoves);
            Player *newPlayer = MakePlayer(name, completedLevels, totalMoves, averageMoves);
            if (*players != NULL) (*players)->back = newPlayer;
            newPlayer->next = *players;
            newPlayer->back = NULL;
            *players = newPlayer;
        }
    }
    fclose(fp);
}
Player *MakePlayer(char name[], int completedLevels, int totalMoves, int averageMoves){
    Player *uj = (Player *) malloc(sizeof(Player));
    strcpy(uj->name, name);
    uj->completedLevels = completedLevels;
    uj->totalMoves = totalMoves;
    uj->averageMoves;
    uj->next = NULL;
    uj->back = NULL;
    return uj;
}
void FreePlayerList(Player *playerlist){
    while (playerlist != NULL) {
        Player *temp = playerlist->next;
        free(playerlist);
        playerlist = temp;
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
/*int ToIndex(Point p){
    return p.x + p.y * totalWidth;
}
Point ToPos(int index){
    return (Point){index % totalWidth, index / totalWidth};
}*/
