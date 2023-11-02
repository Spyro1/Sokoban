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
#include <windows.h>

/** Feladatok
 * KÉSZ: MainScreen-ben a visszakapott paraméter legyen a selectedLevelFileName helyett a levelFileNameList, hogy lehessen választani és továbbl éptetni a szinteket a játék során.
 * Level függvények modulosítása, hogy csak a main fgv maradjon
 * KÉSZ: XSB fájl beolvasása **map-be
 * KÉSZ: Új játékos hozzáadása, név megadása, listához hozzáfűzése és visszaadása
 * KSÉZ: Player struct átírása, hogy megfeleljen a feladatkiírásnak
 * KÉSZ: Teljes Menü átalakítása a specifikációnak megfellően
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
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);

    // Főképernyő kiiratása és Szint választás
    int selectedLevel = 0; // A menüben kiválasztott szint indexe
    int numOfLevels; // Dinamikus levelList tömb hossza
    Player currentPlayer; // A menüben kiválasztott játékos


    // Főképernyő meghívása
    MainScreen(&currentPlayer, numOfLevels, &selectedLevel);
    // Próba
//    currentPlayer = (Player) {"Marci", 6};
//    if (currentPlayer.numOfCompletedLevels > 0){
//        selectedLevel = currentPlayer.numOfCompletedLevels-1;
//    }
//    else perror("Nem elfogadhato a player szintje.");
    // Próba

    // Játék inicializálása
//    Init(&currentPlayer, levelList, numOfLevels, selectedLevel);
//    printf("\n\nJáték Fut!\n\n");
//

    debugmalloc_log_file("debugmalloc.txt");
    return 0;
}

void MainScreen(Player *currentPlayer, int numOfLevels, int *selectedLevel){

    // FŐCÍM_KIÍRÁSA
    PrintTitle();

    // Konstansok a kiiratáshoz
    const int center = 36;

    const char strNewPlayer[] = "ÚJ JÁTÉKOS";
    const char strChosePlayer[] = "JÁTÉKOS VÁLASZTÁS";
    const char strRankList[] = "DICSŐSÉGLISTA";
    const char strExitApp[] = "KILÉPÉS";
    Point p = {center, 9};
    // Paraméterek inicializálása
    currentPlayer = NULL;
    *selectedLevel = 0;
    // Menü Változók
    char **levelList = NULL; // Dinamikus tömb lesz
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
        p = (Point) {center, 9};
        prevOption = option;
        // Lenyomott_billentyű_kiértékelése
        switch (key){
            case KEY_ESCAPE:
            case KEY_BACKSPACE:
                if (state == mainMenu) state = exitApp;
                else state = mainMenu;
                // Reset Menü
                ResetMenuVars(&displayFirst, &option, &selectedPlayer);
                break;
            case KEY_ENTER:
                if      (state == exitApp && option == 1) runMenu = false; // Kilépés a menüből
                else if (state == exitApp) {
                    state = mainMenu;
                    ResetMenuVars(&displayFirst, &option, &selectedPlayer);
                }
                else if (state == mainMenu){
                    state = mainMenu + 1 + option;
                    ResetMenuVars(&displayFirst, &option, &selectedPlayer);
                }
                else if (state == chosePlayer && currentPlayer != NULL){
                    // Előkészítés
                    // Szint Mappa beolvasása
                    ReadDirectoryLevelNames("./levels/", &levelList, &numOfLevels);
                    while(Init(currentPlayer, levelList, numOfLevels, *selectedLevel)) {
//                        econio_clrscr();
//                        PrintTitle();
                        *selectedLevel = currentPlayer->numOfCompletedLevels;
                    }
                    econio_clrscr();
                }
                // Reset Menü
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
                else if (state == chosePlayer && selectedPlayer > 0)
                    selectedPlayer--;
                break;
            case KEY_DOWN:
                if (state == mainMenu && option < exitApp-1)
                    option++;
                else if (state == chosePlayer && selectedPlayer < numOfPlayers-1)
                    selectedPlayer++;
                break;
            default: break;
        }

        // Képernyőre_írás_mód_szerint

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
                    //scanf("%s[^\n]", newPlayerName);
                    if (newPlayerName[0] != '\n'){
                        newPlayerName[strlen(newPlayerName)-1] = '\0';
                         // Fájlba írás
                        player_ReadTxtFile(&PlayerList, &numOfPlayers);
                        int exists = player_GetIndexOfPlayer(PlayerList, newPlayerName);
                        if (exists == -1) {
                            player_AddPlayerToEnd(player_MakePlayer(newPlayerName,0,NULL), &PlayerList, &numOfPlayers);
                            player_WriteTxtFile(PlayerList, numOfPlayers);
                        }
                    }
                    state = mainMenu;
                    key = -1;
                }
                break;
            case chosePlayer:
                if (displayFirst){
                    // Kiiratás inicializálása
                    displayFirst = false;

                    ClearScrBellow();
                    // Alcím kiírása
                    printfc("JÁTÉKOSOK:", p.x-5, p.y, baseForeColor);
                    // Fájl beolvasása
                    player_ReadTxtFile(&PlayerList, &numOfPlayers);
                }
                player_PrintPlayerList(PlayerList, selectedPlayer, (Point) {p.x,p.y+1} );
                currentPlayer = player_GetSelectedPlayer(PlayerList, selectedPlayer);
                if (currentPlayer != NULL) *selectedLevel = currentPlayer->numOfCompletedLevels;
                break;

            case rankList:
                if (displayFirst){
                    displayFirst = false;
                    ClearScrBellow();
                    p = (Point) {12,p.y};
//                    printfc("Dicsőséglista", p.x, p.y, COL_CYAN);
                    player_ReadTxtFile(&PlayerList, &numOfPlayers);
                    PrintRankList(PlayerList, numOfPlayers, p);
                }
                break;
            default: break;
        }

        if (runMenu && !displayFirst) key = econio_getch(); // Billentyűlenyomásra vár, ha fut a menü
    }
    // Játékosok adatainak mentése
    player_WriteTxtFile(PlayerList, numOfPlayers);
    // Játékosok listájának felszabadítása
    player_FreePlayerList(&PlayerList); // Player lista felszabadítása
    // Lefoglalt levelLista felszabadítása
    FreeLevelList(&levelList, &numOfLevels);
}

void ResetMenuVars(bool *displayFirst, int *option, int *selectedPlayer) {
    *displayFirst = true;
    *option = 0;
    *selectedPlayer = 0;
}

void PrintTitle(){
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
    printfc(             "↑    ⌫", 16, 21,COL_LIGHTCYAN);
    printfc("Navigálás: ← ↓ →  ↲", 3 , 22,COL_LIGHTCYAN);
}
