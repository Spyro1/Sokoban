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
//#ifdef _WIN32
#include <windows.h>
//#endif

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
    // Encoding beállítások
//    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
//    #endif
//    setlocale(LC_ALL, "");
//    fwide(stdout, 1);
//    fputwc(L'\uFEFF', stdout);

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
//    currentPlayer = (Player) {"Marci", 2};
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
//    char t[1];
//    scanf("%c", t);
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
    const int center = 36;
    // Navigáiós Gombok kiiratása ak épernyő aljára
    econio_textcolor(COL_LIGHTCYAN);
    econio_gotoxy(16,21); printf("↑    ⌫");
    econio_gotoxy(3,22); printf("Navigálás: ← ↓ →  ↲");

    // Konstansok a kiiratáshoz


    const char strNewPlayer[] = "ÚJ JÁTÉKOS";
    const char strChosePlayer[] = "JÁTÉKOS VÁLASZTÁS";
    const char strRankList[] = "DICSŐSÉGLISTA";
    const char strExitApp[] = "KILÉPÉS";

    // Paraméterek inicializálása
    currentPlayer = NULL;
    *selectedLevel = 0;
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
    char newPlayerName[nameLenght];
    int newPlayerNameLenght = 0;
    enum State { mainMenu, newPlayer, chosePlayer, rankList, exitApp };
    enum State state = mainMenu;

    econio_rawmode();
    // Menü futtatása
    while(runMenu){
        prevOption = option;
        // Lenyomott_billentyű_kiértékelése
        switch (key){
            case KEY_ESCAPE:
            case KEY_BACKSPACE:
                if (state == mainMenu) state = exitApp;
                else state = mainMenu;
                // Reset Menü
                displayFirst = true;
                option = 0;
                break;
            case KEY_ENTER:
                if (state == exitApp && option == 1) runMenu = false; // Kilépés a menüből
                else if (state == exitApp) state = mainMenu;
                else {
                    state = mainMenu + 1 + option;
                }
                // Reset Menü
                displayFirst = true;
                option = 0;
                break;
            case KEY_RIGHT:
                if (state == exitApp && option == 0)
                    option = 1;
                break;
            case KEY_LEFT:
                if (state == exitApp && option == 1)
                    option = 0;
                break;
            case KEY_UP:
                if (state == mainMenu && option > 0)
                    option--;
                break;
            case KEY_DOWN:
                if (state == mainMenu && option < exitApp-1)
                    option++;
                break;
            default: break;
        }

        // Képernyőre_írás_mód_szerint
        Point p = {center, 9};
        switch (state) {
            case exitApp:
                p = (Point) {22, 9};
                // Ha fut a menü
                if (runMenu){
                    if (displayFirst) {
                        displayFirst = false;
                        ClearScrBellow();
                        printfc("╔════════════════════════╗", p.x,p.y+0,COL_RED);
                        printfc("║    BIZTOSAN KILÉPSZ?   ║", p.x,p.y+1,COL_RED);
                        printfc("║                        ║", p.x,p.y+2,COL_RED);
                        printfc("║     NEM       IGEN     ║", p.x,p.y+3,COL_RED);
                        printfc("╚════════════════════════╝", p.x,p.y+4,COL_RED);
                    } // Kilépő ablak kiírása
                    if (option == 0) {
                        printfbc("NEM", p.x+6,p.y+3, COL_WHITE, COL_LIGHTRED);
                        printfbc("IGEN", p.x+16,p.y+3, COL_RED, COL_RESET);
                    }
                    else {
                        printfbc("NEM", p.x+6,p.y+3, COL_RED, COL_RESET);
                        printfbc("IGEN", p.x+16,p.y+3, COL_WHITE, COL_LIGHTRED);
                    }
                }
                else {
                    econio_clrscr();
                    printfc("LEÁLLÍTÁS", 33, 10, COL_RED);
                } // Leíállítás kiírása
                break;
            case mainMenu:
                if (displayFirst){
                    displayFirst = false;
                    ClearScrBellow();
                    printfc(strNewPlayer, p.x - (int)strlen(strNewPlayer)/2, p.y, baseForeColor); // 0
                    printfc(strChosePlayer, p.x - (int)strlen(strChosePlayer)/2, p.y+1, baseForeColor); // 0
                    printfc(strRankList, p.x - (int)strlen(strRankList)/2, p.y+2, baseForeColor); // 0
                    printfc(strExitApp, p.x - (int)strlen(strExitApp)/2, p.y+3, baseForeColor); // 0
                }
                // Előző opcó deaktiválása
                switch (prevOption) {
                    case 0:
                        printfc(strNewPlayer, p.x - (int)strlen(strNewPlayer)/2, p.y, baseForeColor); // 0
                        break;
                    case 1:
                        printfc(strChosePlayer, p.x - (int)strlen(strChosePlayer)/2, p.y+1, baseForeColor); // 0
                        break;
                    case 2:
                        printfc(strRankList, p.x - (int)strlen(strRankList)/2, p.y+2, baseForeColor); // 0
                        break;
                    case 3:
                        printfc(strExitApp, p.x - (int)strlen(strExitApp)/2, p.y+3, baseForeColor); // 0
                        break;
                    default: break;
                }
                // Aktuális opció kiemelésa
                switch (option){
                    case 0:
                        printfbc(strNewPlayer,p.x - (int)strlen(strNewPlayer)/2, p.y, activeForeColor, activeBgColor); // 0
                        break;
                    case 1:
                        printfbc(strChosePlayer, p.x - (int)strlen(strChosePlayer)/2, p.y+1, activeForeColor, activeBgColor); // 0
                        break;
                    case 2:
                        printfbc(strRankList, p.x - (int)strlen(strRankList)/2, p.y+2, activeForeColor, activeBgColor); // 0
                        break;
                    case 3:
                        printfbc(strExitApp, p.x - (int)strlen(strExitApp)/2, p.y+3, activeForeColor, activeBgColor); // 0
                        break;
                    default: break;
                }
                break;
            case newPlayer:
                if (displayFirst){
                    // Kiiratás inicializálása
                    ClearScrBellow();
                    // Alcím kiírása
                    printfc("Név (max 20 karakter):", center-11, p.y, baseForeColor);
                    printfc("____________________", center-10, p.y+1, baseForeColor);
                    econio_gotoxy(center-10, p.y+1);
                    fgets(newPlayerName, nameLenght, stdin);

                    if (newPlayerName[0] != '\n'){
                         // Fájlba írás
                        // Még átdolgozást igényel!!!
                    }
                    state = mainMenu;
                    key = -1;
                }
                break;
            /*case chosePlayer:
                _x = 22; _y = 9;

                    if (displayFirst){
                        // Kiiratás inicializálása
                        displayFirst = false;
                        ClearScrBellow();
                        FreeLevelList(&levelFileNames, numOfLevels);
                        //int h = ((int)strlen(currentPlayer->name))/2;
                        econio_gotoxy(_x-1 ,_y-1);
                        econio_textcolor(COL_LIGHTCYAN);
                        // currentPlayer Kiválasztása a láncolt Listából selectedPlayer alapján
                        currentPlayer = player_GetSelectedPlayer(PlayerList, selectedPlayer);
                        // Alcím kiírása
                        printf("%s TELJESÍTETT SZINTJEI:", currentPlayer->name);
                        // printf("SZINTEK:");
                        // Szintek beolvasása
                        ReadDirectoryLevelNames("./levels/", &levelFileNames, numOfLevels);
                        *selectedLevel = currentPlayer->completedLevels-1;
                    }
                    PrintLevels(levelFileNames, *numOfLevels, *selectedLevel, currentPlayer->completedLevels, (Point) {_x+9, _y});

                break;*/
            case chosePlayer:
                if (displayFirst){
                    // Kiiratás inicializálása
                    displayFirst = false;
                    player_FreePlayerList(&PlayerList);
                    ClearScrBellow();
                    // Alcím kiírása
                    printfc("JÁTÉKOSOK:", p.x-5, p.y, baseForeColor);
                    // Fájl beolvasása
                    player_ReadTxtFile(&PlayerList, &numOfPlayers);
                }
                player_PrintPlayerList(PlayerList, selectedPlayer, (Point) {p.x,p.y+1} );
                break;

            case rankList:
                if (displayFirst){
                    displayFirst = false;
                    ClearScrBellow();
                    printfc("Dicsőséglista", 12, 9, COL_CYAN);
                }
                break;
            default: break;
        }


//        if (!(state == choseLevel && selectedPlayer ==0) ) econio_gotoxy(0,0); // Kurzor bal felső sarokba
        if (runMenu && !displayFirst) key = econio_getch(); // Billentyűlenyomásra vár, ha fut a menü
        //if (!runMenu) returnerPlayer = *currentPlayer; // Player kimentése
    }

    // Free dinamikus tömbök
    player_FreePlayerList(&PlayerList); // Player lista felszabadítása
    *currentPlayer = returnerPlayer; // Kifelé menő paraméter megadása a segéd változóból
}

void ClearScrBellow(){
    ClearScreenSection(0, 8, 73, 22, COL_RESET);
}
void ClearScreenSection(int x1, int y1, int x2, int y2, EconioColor bgColor){
    econio_gotoxy(x1,y1);
    econio_textcolor(COL_RESET);
    econio_textbackground(bgColor);
    for (int i = 0; i < y2-y1; i++){
        for(int j = 0; j < x2-x1; j++){
            printf(" ");
        }
        econio_gotoxy(x1,y1+i);
    }
}
void print(char const str[], int x, int y){
    econio_textbackground(COL_RESET);
    econio_gotoxy(x,y);
    printf("%s", str);
}
void printfc(char const str[], int x, int y, EconioColor foreColor){
    econio_textbackground(COL_RESET);
    econio_gotoxy(x,y);
    econio_textcolor(foreColor);
    printf("%s", str);
}
void printfbc(char const str[], int x, int y, EconioColor foreColor, EconioColor bgColor){
    econio_textbackground(COL_RESET);
    econio_gotoxy(x,y);
    econio_textcolor(foreColor);
    econio_textbackground(bgColor);
    printf("%s", str);
    econio_textbackground(COL_RESET);
}

int* Split(char *text, char splitter, int count){
    int *newArray = (int*) malloc(count * sizeof(int));
    char* helper;
    strcpy(helper,text);
    int item;
    for (int i = 0; i < count; ++i) {
        sscanf(helper,"%d;%s[\n]", &item, helper);
        newArray[i] = item;
    }
    return newArray;
}

