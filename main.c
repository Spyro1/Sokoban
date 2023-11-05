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
    // Karakter kódolás beállítása
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    // Főmenü meghívása
    // Csak a játék indításához kommenteld a következő sort
     MainScreen();

    // == TESZT == : Csak a játék indításához uncoment-eld ezt a részt TESZT END kommentig
//        // Változók deklarálása
//        int numOfLevels; // A beolvasott szintek száma a ./levels/ mappából, levelList tömb hossza
//        Player currentPlayer; // A menüben kiválasztott játékost tartalmazza
//        char **levelList; // A szintek fájlneveit tartalmazza a ./levels/ mappából, dinamikus string tömb
//        // Szintek beolvasása a ./levels/ mappából
//        ReadDirectoryLevelNames("./levels/", &levelList, &numOfLevels);
//        currentPlayer = (Player) {"Marci", 1};
//        // Játék inicializálása
//        Init(&currentPlayer, levelList, numOfLevels);
    // == TESZT END ==

    // Debugmalloc loggolása fájlba
    debugmalloc_log_file("debugmalloc.txt");
    return 0;
}

void MainScreen(){

    // Főcím kiiratása "SOKOBAN"
    PrintTitle();

    // Paraméterek inicializálása

    // Konstansok a kiiratáshoz
    const int center = 36; // Képernyő közepe a cím szerint
    Point p = {center, 9}; // A kiiratás középpontja a cím alatt

    const char strNewPlayer[] = "ÚJ JÁTÉKOS";
    const char strChosePlayer[] = "JÁTÉKOS VÁLASZTÁS";
    const char strRankList[] = "DICSŐSÉGLISTA";
    const char strExitApp[] = "KILÉPÉS";

    // Menü Változók
    Player *currentPlayer = NULL;   // Aktuálisan kiválasztott játékost tároló pointer
    int selectedLevel = 0;          // Kiválasztott szint
    char **levelList = NULL;        // A szintek fájlneveit tartalmazza a ./levels/ mappából, dinamikus string tömb
    int numOfLevels = 0;            // A beolvasott szintek száma a ./levels/ mappából, levelList tömb hossza
    int key = -1;                   // Lenyomott billentyű kódja
    bool displayFirst = true;       // Igaz, ha egy menüponba először megyünk bele, Hamis, ha már a többedjére
    bool runMenu = true;            // Amíg igaz, addig fut a menü ciklusa, hamisnál leáll a menü, és kilép a programból

    int option = 0;                 // Aktuálisan kiválasztott opció
    int prevOption = 0;             // Előző választott opció
    int numOfPlayers = 0;           // A Játékosok száma a playerListHead lácnolt listába
    int selectedPlayer = 0;         // Az aktuálisan kiválasztott játékos indexe a láncolt listában és a menüben

    Player *playerListHead = NULL;  // A Játékosok adatait tároló dinamikus láncolt lista
    Player returnerPlayer;          //
    char newPlayerName[nameLenght]; // Új játékos hozzáadásakor ebbe kerül a név
    int newPlayerNameLenght = 0;    // Új játkos nevének hossza
    // A menü állapotait leíró enum
    enum State { mainMenu, newPlayer, chosePlayer, rankList, exitApp };
    enum State state = mainMenu;    // Induláskor a főmenü jelenik meg

    econio_rawmode(); // Billentyűérzékelés bekapcsolása

    // == MENÜ FUTTATÁSA ==
    while(runMenu){
        p = (Point) {center, 9};
        prevOption = option;
        // Lenyomott billentyű kiértékelése
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
                else if (state == exitApp) { // Viszzalépés a főmenübe
                    state = mainMenu;
                    ResetMenuVars(&displayFirst, &option, &selectedPlayer);
                }
                else if (state == mainMenu){
                    state = mainMenu + 1 + option; // Főmenüből kijelölt opcióbaváltás
                    ResetMenuVars(&displayFirst, &option, &selectedPlayer);
                }
                else if (state == chosePlayer && currentPlayer != NULL){
                    // Szint Mappa beolvasása
                    ReadDirectoryLevelNames("./levels/", &levelList, &numOfLevels);
                    // Játék indítása
                    while(Init(currentPlayer, levelList)) {
                        // Nem kell ide semmi, addig fut, amíg a játékból ki nem lépnek
                        // vagy nem teljesíti az összes szintet a játékos
                    }
                    ResetMenuVars(&displayFirst, &option, &selectedPlayer);
                }
                break;
            // Navigációk a menüben
            case KEY_RIGHT:
                if (state == exitApp && option == 0)
                    option = 1; // IGEN/NEM választása a kilépésnél
                break;
            case KEY_LEFT:
                if (state == exitApp && option == 1)
                    option = 0; // IGEN/NEM választása a kilépésnél
                break;
            case KEY_UP:
                if (state == mainMenu && option > 0)
                    option--; // Felfele lépés a főmenüben
                else if (state == chosePlayer && selectedPlayer > 0)
                    selectedPlayer--; // Felfele lépés a játékosneveken
                break;
            case KEY_DOWN:
                if (state == mainMenu && option < exitApp-1)
                    option++; // Lefele lépés a főmenüben
                else if (state == chosePlayer && selectedPlayer < numOfPlayers-1)
                    selectedPlayer++; // Lefele lépés a játékosneveken
                break;
            default: break;
        }

        // Képernyőre írás választott mód szerint
        switch (state) {
            case exitApp: // Kilépési ablak
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
                    // Leíállítás kiírása
                    econio_clrscr();
                    printfc("LEÁLLÍTÁS", 33, 10, COL_RED);
                }
                break;
            case mainMenu: // Főmenü kiiratása
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
            case newPlayer: // Új játékos felvétele
                 if (displayFirst){
                    ClearScrBellow();
                    // Alcím kiírása
                    printfc("Név (max 20 karakter):", center-11, p.y, baseForeColor);
                    printfc("____________________", center-10, p.y+1, baseForeColor);
                    econio_gotoxy(center-10, p.y+1);
                    // Bemenet várása a felhasználótól
                    fgets(newPlayerName, nameLenght, stdin);

                    // Ha nem egy üres sort írt be, akkor eltároljuk az új játékost
                    // Ezen még kell finomítani a különböző hibaesetekre, pl üres sor
                    if (newPlayerName[0] != '\n'){
                        newPlayerName[strlen(newPlayerName)-1] = '\0';
                        // Player Listába beolvasás fájlból
                        player_ReadTxtFile(&playerListHead, &numOfPlayers);
                        // Játékosnév tesztelése, hogy létezik e már az adatbázisban
                        int exists = player_GetIndexOfPlayer(playerListHead, newPlayerName);
                        if (exists == -1) {
                            // Új játékos hozzáaadása a láncolt listához
                            player_AddPlayerToEnd(player_MakePlayer(newPlayerName,0,NULL), &playerListHead, &numOfPlayers);
                            // Player Láncoltl ista fájlba mentése
                            player_WriteTxtFile(playerListHead, numOfPlayers);
                        }
                    }
                    state = mainMenu;
                    key = -1;
                }
                break;
            case chosePlayer: // Játékos választás kiiratása
                if (displayFirst){
                    displayFirst = false;
                    ClearScrBellow();
                    // Alcím kiírása
                    printfc("JÁTÉKOSOK:", p.x-5, p.y, baseForeColor);
                    // Player Listába beolvasás fájlból
                    player_ReadTxtFile(&playerListHead, &numOfPlayers);
                }
                // Player Lista kiiratása a képernyőre
                player_PrintPlayerList(playerListHead, selectedPlayer, (Point) {p.x, p.y + 1} );
                // Kijelölt játékos a currentPlayerbe rakása, hogy a játék indítható legyen
                currentPlayer = player_GetSelectedPlayer(playerListHead, selectedPlayer);
                break;
            case rankList: // Dicsőséglista kiiratása
                if (displayFirst){
                    displayFirst = false;
                    ClearScrBellow();
                    p = (Point) {12,p.y};
                    player_ReadTxtFile(&playerListHead, &numOfPlayers);
                    PrintRankList(playerListHead, numOfPlayers, p);
                }
                break;
            default: break;
        }

        if (runMenu && !displayFirst) key = econio_getch(); // Billentyűlenyomásra vár, ha fut a menü
    }
    // Játékosok adatainak mentése
    player_WriteTxtFile(playerListHead, numOfPlayers);
    // Játékosok listájának felszabadítása
    player_FreePlayerList(&playerListHead); // Player lista felszabadítása
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
