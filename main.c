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
#include "level.h"
#ifdef _WIN32
    #include <windows.h>
#endif

/** Feladatok
 * KÉSZ: MainScreen-ben a visszakapott paraméter legyen a selectedLevelFileName helyett a levelFileNameList, hogy lehessen választani és továbbl éptetni a szinteket a játék során.
 * Level függvények modulosítása, hogy csak a main fgv maradjon
 * KÉSZ: XSB fájl beolvasása **map-be
 * Új játékos hozzáadása, név megadása, listához hozzáfűzése és visszaadása
 * Player struct átírása, hogy megfeleljen a feladatkiírásnak
 * Teljes Menü átalakítása a specifikációnak megfellően
 * MENÜ:
 * - ÚJ JÁTÉKOS
 *      - max 20 karakter
 * - JÁTÉKOS VÁLASZTÁS
 *      - Játékosok kilistázása (kurzorral léptetés)
 *      - Visszalép legalsó opció
 * - DICSŐSÉGLISTA
 *      - Táblázat kiírása
 * - KILÉPÉS #
 * */

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    // Főképernyő kiiratása és Szint választás
    int selectedLevel = 0; // A menüben kiválasztott szint indexe
    int numOfLevels; // Dinamikus levelList tömb hossza
    char **levelList; // Dinamikus tömb lesz
    Player currentPlayer; // A menüben kiválasztott játékos

    // Szint Mappa beolvasása
    ReadDirectoryLevelNames("./levels/", &levelList, &numOfLevels);
    // Főképernyő meghívása
    MainScreen(&currentPlayer, numOfLevels, &selectedLevel);
    // Próba
//    currentPlayer = (Player) {"Marci", 7};
//    if (currentPlayer.completedLevels > 0){
//        selectedLevel = currentPlayer.completedLevels-1;
//    }
//    else perror("Nem elfogadhato a player szintje.");
    // Próba

    // Játék inicializálása
//    Init(&currentPlayer, levelList, numOfLevels, selectedLevel);
    printf("Játék Fut!");

    // Lefoglalt levelLista felszabadítása
    FreeLevelList(&levelList, &numOfLevels);
    debugmalloc_log_file("debugmalloc.txt");
    char *t;
    scanf("%s", t);
    return 0;
}

void MainScreen(Player *currentPlayer, int numOfLevels, int *selectedLevel){

    // FŐCÍM_KIÍRÁSA
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

    // Navigáiós Gombok kiiratása ak épernyő aljára
    econio_textcolor(COL_LIGHTCYAN);
    econio_gotoxy(16,21); printf("↑    ⌫");
    econio_gotoxy(3,22); printf("Navigálás: ← ↓ →  ↲");

    // Paraméterek inicializálása
    //*numOfLevels = 0;
    currentPlayer = NULL;
    *selectedLevel = 0;
    //*levelList = NULL;
    // Menü Változók
    int key = -1;
    bool displayFirst = true;
    bool runMenu = true;

    int option = 0, prevOption = 0;
    int numOfPlayers = 0;
    int selectedPlayer = 0;
   // char **levelFileNames = NULL; // String array
    Player *PlayerList = NULL;
    Player returnerPlayer;
    char newPlayerName[21];
    enum State { exitApp, mainMenu, newPlayer, chosePlayer, rankList, startLvl };
    enum State state = chosePlayer;

    econio_rawmode();
    // Menü futtatása
    while(runMenu){

        // Lenyomott_billentyű_kiértékelése
        switch (key){
            case KEY_ESCAPE:
            case KEY_BACKSPACE:
                if (state > exitApp) state--; // Módváltás
                else if (state == exitApp) state++;
                // Reset Menü
                displayFirst = true;
                option = 0;
                break;
            case KEY_ENTER:
                if (state == exitApp && option == 0) { runMenu = false; }
                if (state < startLvl) state++; // Módváltás
                // Reset Menü
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
                //if (state == choseLevel && *selectedLevel > 0) (*selectedLevel)--;
                break;
            case KEY_DOWN:
                if (state == chosePlayer && selectedPlayer < numOfPlayers-1){
                    selectedPlayer++;
                }
                //if (state == choseLevel && *selectedLevel < currentPlayer->completedLevels-1) (*selectedLevel)++;
                break;
        }

        // Képernyőre_írás_mód_szerint
        Point p;
        switch (state) {
            case exitApp:
                p = (Point) {25, 8}; //_x = 25, _y = 8;
                if (displayFirst) {
                    displayFirst = false;
                    ClearScrBellow();
                    econio_textcolor(COL_RED);
                    econio_gotoxy(p.x, p.y+1);
                    printf("╔════════════════════════╗\n");
                    econio_gotoxy(p.x, p.y+2);
                    printf("║    BIZTOSAN KILÉPSZ?   ║\n");
                    econio_gotoxy(p.x, p.y+3);
                    printf("║                        ║\n");
                    econio_gotoxy(p.x, p.y+4);
                    printf("║    IGEN        NEM     ║\n");
                    econio_gotoxy(p.x, p.y+5);
                    printf("╚════════════════════════╝\n");
                }
                if (option == 0) {
                    econio_gotoxy(p.x+5, p.y+4);
                    econio_textcolor(COL_WHITE); econio_textbackground(COL_LIGHTRED);
                    printf("IGEN");
                    econio_gotoxy(p.x+17, p.y+4);
                    econio_textcolor(COL_RED); econio_textbackground(COL_RESET);
                    printf("NEM");
                }
                else {
                    econio_gotoxy(p.x+5, p.y+4);
                    econio_textcolor(COL_RED); econio_textbackground(COL_RESET);
                    printf("IGEN");
                    econio_gotoxy(p.x+17, p.y+4);
                    econio_textcolor(COL_WHITE); econio_textbackground(COL_LIGHTRED);
                    printf("NEM");
                }
                break;
            case mainMenu:
                p = (Point) {12, 9};
                if (displayFirst){
                    displayFirst = false;
                    ClearScrBellow();
                    econio_gotoxy(p.x, p.y);
                    printf("Új játékos");
                    econio_gotoxy(p.x, p.y+1);
                    printf("Játékos válaszás");
                    econio_gotoxy(p.x, p.y+2);
                    printf("Dicsőséglista");
                    econio_gotoxy(p.x, p.y+3);
                    printf("Kilépés");
                }

                break;
            case chosePlayer:
                p = (Point) {12, 9};
                if (displayFirst){
                    // Kiiratás inicializálása
                    displayFirst = false;
                    player_FreePlayerList(&PlayerList);
                    ClearScrBellow();
                    econio_gotoxy(p.x+20,p.y-1);
                    econio_textcolor(COL_LIGHTCYAN);
                    // Alcím kiírása
                    printf("JÁTÉKOSOK:");
                    // Fájl beolvasása
                    player_ReadTxtFile(&PlayerList, &numOfPlayers);
                }
                player_PrintPlayerList(PlayerList, numOfPlayers, selectedPlayer, p);
                break;
            case newPlayer:
                if (displayFirst){
                    // Kiiratás inicializálása
                    displayFirst = false;
                    ClearScrBellow();
                    econio_gotoxy(p.x+6, p.y-1);
                    econio_textcolor(COL_LIGHTCYAN);
                    // Alcím kiírása
                    printf("Név (max 20 karakter):");
                    econio_gotoxy(p.x+7,p.y);
                    printf("____________________");
                    econio_gotoxy(p.x+7,p.y);
                    econio_normalmode();
                    if (scanf("%20s[^\n]", newPlayerName) == 1){
                        runMenu = false;
                    }
                    // Még átdolgozást igényel!!!
                    econio_rawmode();
                }
                break;
//            case chosePlayer:
//                _x = 22; _y = 9;
//
//                    if (displayFirst){
//                        // Kiiratás inicializálása
//                        displayFirst = false;
//                        ClearScrBellow();
//                        FreeLevelList(&levelFileNames, numOfLevels);
//                        //int h = ((int)strlen(currentPlayer->name))/2;
//                        econio_gotoxy(_x-1 ,_y-1);
//                        econio_textcolor(COL_LIGHTCYAN);
//                        // currentPlayer Kiválasztása a láncolt Listából selectedPlayer alapján
//                        currentPlayer = player_GetSelectedPlayer(PlayerList, selectedPlayer);
//                        // Alcím kiírása
//                        printf("%s TELJESÍTETT SZINTJEI:", currentPlayer->name);
//                        // printf("SZINTEK:");
//                        // Szintek beolvasása
//                        ReadDirectoryLevelNames("./levels/", &levelFileNames, numOfLevels);
//                        *selectedLevel = currentPlayer->completedLevels-1;
//                    }
//                    PrintLevels(levelFileNames, *numOfLevels, *selectedLevel, currentPlayer->completedLevels, (Point) {_x+9, _y});
//
//                break;
            case rankList:
                break;
            case startLvl:
                runMenu = false;
                returnerPlayer = *currentPlayer;    // Player kimentése
                //*levelList = levelFileNames; // Level lista átcímzése
                //strcpy(selectedLevelFileName, levelFileNames[selectedLevel]);
                break;
        }
//        #pragma endregion Képernyőre_írás_mód_szerint

//        if (!(state == choseLevel && selectedPlayer ==0) ) econio_gotoxy(0,0); // Kurzor bal felső sarokba
        if (runMenu) key = econio_getch(); // Billentyűlenyomásra vár, ha fut a menü
    }

    // Free dinamikus tömbök
    player_FreePlayerList(&PlayerList); // Player lista felszabadítása
    *currentPlayer = returnerPlayer; // Kifelé menő paraméter megadása a segéd változóból
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



