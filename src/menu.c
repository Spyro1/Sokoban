#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../libraries/debugmalloc.h"
#include "../libraries/econio.h"
#include "../headers/datatypes.h"
#include "../headers/menu.h"
#include "../headers/player.h"
#include "../headers/game.h"
#include "../headers/level.h"
#include "../headers/lib.h"


const Size maxSize = {72,30};

void menu_MainScreen(){

    // Konstansok a kiiratáshoz

    const int center = maxSize.width/2; // Képernyő közepe a cím szerint

    Point p;                        // A kiiratás középpontja a cím alatt
    // Menü Változók
    Player *currentPlayer = NULL;   // Aktuálisan kiválasztott játékost tároló pointer
    int key = -1;                   // Lenyomott billentyű kódja
    bool displayFirst = true;       // Igaz, ha egy menüponba először megyünk bele, Hamis, ha már a többedjére
    bool runMenu = true;            // Amíg igaz, addig fut a menü ciklusa, hamisnál leáll a menü, és kilép a programból
    int option = 0;                 // Aktuálisan kiválasztott opció
    int prevOption;                 // Előző választott opció
    int numOfPlayers = 0;           // A Játékosok száma a playerListHead lácnolt listába
    int selectedPlayer = 0;         // Az aktuálisan kiválasztott játékos indexe a láncolt listában és a menüben
    Player *playerListHead = NULL;  // A Játékosok adatait tároló dinamikus láncolt lista
    char **levelList = NULL;        // A szintek fájlneveit tartalmazza a ./levels/ mappából, dinamikus string tömb
    int numOfLevels = 0;            // A beolvasott szintek száma a ./levels/ mappából, levelList tömb hossza
    int linesPrinted = 0;           // A konzolra írt sorok száma egyes kiiratásokor
    State state = mainMenu;    // A menürendszer aktuális állapotát tárolja

    // Főcím kiiratása "SOKOBAN"
    menu_PrintTitle();
    // Szint Mappa beolvasása
    level_ReadDirectory("./levels/", &levelList, &numOfLevels);
    econio_rawmode(); // Billentyűérzékelés bekapcsolása

    // == MENÜ FUTTATÁSA ==
    while(runMenu){
        p = (Point) {center, 9};
        prevOption = option;
        // Lenyomott billentyű kiértékelése
        menu_KeyPress(key, &state, &displayFirst, &option, &selectedPlayer, &runMenu, currentPlayer, &playerListHead, &numOfPlayers, levelList, numOfLevels);
        // Képernyőre írás választott mód szerint
        menu_EvaluateState(&key, &state, runMenu, &displayFirst, option, prevOption, &playerListHead, &currentPlayer, &numOfPlayers, selectedPlayer, p, &linesPrinted);
        // Billentyűlenyomásra vár, ha fut a menü
        if (runMenu && !displayFirst) key = econio_getch();
    }
    // Játékosok adatainak mentése
    player_WriteTxtFile(playerListHead, numOfPlayers);
    // Játékosok listájának felszabadítása
    player_FreePlayerList(&playerListHead); // Player lista felszabadítása
    // Lefoglalt levelLista felszabadítása
    level_FreeLevelList(&levelList, &numOfLevels);
}

static void menu_PrintTitle(){
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
//    printfc(             "↑    ⌫", 16, 24,COL_LIGHTCYAN);
//    printfc("Navigálás: ← ↓ →  ↲", 3 , 25,COL_LIGHTCYAN);
}

static void menu_KeyPress(int key, State *state, bool *displayFirst, int *option, int *selectedPlayer, bool *runMenu, Player *currentPlayer, Player **playerListHead, int *numOfPlayers, char **levelList, int numOfLevels){
    switch (key){
        case KEY_ESCAPE:
        case KEY_BACKSPACE:
            if (*state == mainMenu) *state = exitApp;
            else if (*state == deletePlayer) *state = chosePlayer;
            else if (*state == rankList){
                *state = mainMenu;
                econio_clrscr();
                menu_PrintTitle();
            }
            else *state = mainMenu;
            // Reset Menü
            menu_ResetMenuVars(displayFirst, option, selectedPlayer);
            break;
        case KEY_ENTER:
            switch (*state) {
                case exitApp:
                    if (*option == 1)
                        *runMenu = false; // Kilépés a menüből
                    else{
                        // Viszzalépés a főmenübe
                        *state = mainMenu;
                        menu_ResetMenuVars(displayFirst, option, selectedPlayer);
                    }
                    break;
                case mainMenu:
                    *state = mainMenu + 1 + *option; // Főmenüből kijelölt opcióbaváltás
                    menu_ResetMenuVars(displayFirst, option, selectedPlayer);
                    break;
                case chosePlayer:
                    if (currentPlayer != NULL){
                        *state = game;
                        menu_PrintNavControls(true,*state);
                        // Játék indítása
                        while(game_Init(currentPlayer, levelList) && currentPlayer->numOfCompletedLevels < numOfLevels) {
                            // Nem kell ide semmi, addig fut, amíg a játékból ki nem lépnek
                            // vagy nem teljesíti az összes szintet a játékos
                        }
                        if (currentPlayer->numOfCompletedLevels == numOfLevels) *state = winGame;
                        else *state = chosePlayer;
                        player_WriteTxtFile(*playerListHead, *numOfPlayers);
                        menu_ResetMenuVars(displayFirst, option, selectedPlayer);
                    }
                    break;
                case deletePlayer:
                    if (*option == 1){
                        player_RemovePlayer(currentPlayer,playerListHead, numOfPlayers);
                        player_WriteTxtFile(*playerListHead, *numOfPlayers);
                    }
                    *state = chosePlayer;
                    menu_ResetMenuVars(displayFirst, option, selectedPlayer);
                    break;
                default: break;
            }
            break;
            // Navigációk a menüben
        case KEY_RIGHT:
            if (*state >= exitApp && *option == 0)
                *option = 1; // IGEN/NEM választása a kilépésnél
            //else if (*state == rankList) page->y++; // Dicsőséglistánál lapozás jobbra
            break;
        case KEY_LEFT:
            if (*state >= exitApp && *option == 1)
                *option = 0; // IGEN/NEM választása a kilépésnél
            //else if (*state == rankList && page->y > 0) page->y--; // Dicsőséglistánál lapozás balra
            break;
        case KEY_UP:
            if (*state == mainMenu && *option > 0)
                (*option)--; // Felfele lépés a főmenüben
            else if (*state == chosePlayer && *selectedPlayer > 0)
                (*selectedPlayer)--; // Felfele lépés a játékosneveken
            //else if (*state == rankList && page->x > 0) page->x--; // Dicsőséglistánál lapozás felfele
            break;
        case KEY_DOWN:
            if (*state == mainMenu && *option < exitApp-1)
                (*option)++; // Lefele lépés a főmenüben
            else if (*state == chosePlayer && *selectedPlayer < (*numOfPlayers)-1)
                (*selectedPlayer)++; // Lefele lépés a játékosneveken
            //else if (*state == rankList) page->x++; // Dicsőséglistánál lapozás lefele
            break;
        case 'E':
        case 'e':
        case KEY_F2: // Játékos név szerkesztése
            if (*state == chosePlayer){
                *state = editPlayer;
                *displayFirst = true;
            }
            break;
        case 'D':
        case 'd':
        case KEY_DELETE:// Játékos eltávolítása
            if (*state == chosePlayer){
                *state = deletePlayer;
                menu_ResetMenuVars(displayFirst, option, selectedPlayer);
                *displayFirst = true;
            }
            break;
        default: break;
    }
}

static void menu_EvaluateState(int *key, State *state, bool runMenu, bool *displayFirst, int option, int prevOption, Player **playerListHead, Player **currentPlayer, int *numOfPlayers, int selectedPlayer, Point p, int *linesPrinted){
    menu_PrintNavControls(*displayFirst, *state);
    switch (*state) {
        case exitApp: // Kilépési ablak
            menu_PrintExitWindow(runMenu, displayFirst, option, p);
            *linesPrinted = 6;
            break;
        case mainMenu: // Főmenü kiiratása
            menu_PrintMainMenu(displayFirst, option, prevOption, p);
            *linesPrinted = 4;
            break;
        case newPlayer: // Új játékos felvétele
            menu_PrintNewPlayerSubMenu(state, playerListHead, numOfPlayers, selectedPlayer, p);
            *linesPrinted = 4;
            *key = -1;
            break;
        case chosePlayer: // Játékos választás kiiratása
            menu_PrintPlayerSubMenu(displayFirst, playerListHead, numOfPlayers, currentPlayer, selectedPlayer, p);
            *linesPrinted = *numOfPlayers + 1;
            break;
        case deletePlayer:
            lib_WarningWindow("BIZTOSAN TÖRLÖD?", p, displayFirst, option, baseForeColor, activeForeColor,activeBgColor);
            *linesPrinted = 6;
            break;
        case editPlayer:
            menu_PrintNewPlayerSubMenu(state, playerListHead, numOfPlayers, selectedPlayer, p);
            *linesPrinted = 4;
            *displayFirst = true;
            *key = -1;
            break;
        case rankList: // Dicsőséglista kiiratása
            menu_PrintRankList(displayFirst, playerListHead, numOfPlayers, p, maxDisplayLines);
            *linesPrinted = maxDisplayLines;
            break;
        case winGame:
            lib_ClearScrBellow();
            printfc("TELJESÍTETTED AZ ÖSSZES SZINTEN!", p.x-16, p.y+3,COL_LIGHTGREEN);
            printfc("GRATULÁLOK!", p.x-6, p.y+4,COL_LIGHTYELLOW);
            break;
        default: break;
    }
}

static void menu_PrintExitWindow(bool runMenu, bool *displayFirst, int option, Point p) {
    // Ha fut a menü
    if (runMenu){
        lib_WarningWindow("BIZTOSAN KILÉPSZ?", p, displayFirst, option, COL_RED, COL_WHITE, COL_LIGHTRED);
    }
    else {
        // Leíállítás kiírása
        econio_clrscr();
        printfc("LEÁLLÍTÁS", 33, 10, COL_RED);
        econio_sleep(waitsec);
    }
}

static void menu_PrintMainMenu(bool *displayFirst, int option, int prevOption, Point p) {
    if (*displayFirst){
        *displayFirst = false;
        lib_ClearScrBellow();
        printfc(strNewPlayer, p.x - (int)stringlenght(strNewPlayer)/2, p.y, baseForeColor); // 0
        printfc(strChosePlayer, p.x - (int)stringlenght(strChosePlayer)/2, p.y+1, baseForeColor); // 0
        printfc(strRankList, p.x - (int)stringlenght(strRankList)/2, p.y+2, baseForeColor); // 0
        printfc(strExitApp, p.x - (int)stringlenght(strExitApp)/2, p.y+3, baseForeColor); // 0
    }
    // Előző opcó deaktiválása
    switch (prevOption) {
        case 0:
            printfc(strNewPlayer, p.x - (int)stringlenght(strNewPlayer)/2, p.y, baseForeColor); // 0
            break;
        case 1:
            printfc(strChosePlayer, p.x - (int)stringlenght(strChosePlayer)/2, p.y+prevOption, baseForeColor); // 0
            break;
        case 2:
            printfc(strRankList, p.x - (int)stringlenght(strRankList)/2, p.y+prevOption, baseForeColor); // 0
            break;
        case 3:
            printfc(strExitApp, p.x - (int)stringlenght(strExitApp)/2, p.y+prevOption, baseForeColor); // 0
            break;
        default: break;
    }
    // Aktuális opció kiemelésa
    switch (option){
        case 0:
            printfbc(strNewPlayer,p.x - (int)stringlenght(strNewPlayer)/2, p.y, activeForeColor, activeBgColor); // 0
            break;
        case 1:
            printfbc(strChosePlayer, p.x - (int)stringlenght(strChosePlayer)/2, p.y+option, activeForeColor, activeBgColor); // 0
            break;
        case 2:
            printfbc(strRankList, p.x - (int)stringlenght(strRankList)/2, p.y+option, activeForeColor, activeBgColor); // 0
            break;
        case 3:
            printfbc(strExitApp, p.x - (int)stringlenght(strExitApp)/2, p.y+option, activeForeColor, activeBgColor); // 0
            break;
        default: break;
    }
}

static void menu_PrintNewPlayerSubMenu(State *state, Player **playerListHead, int *numOfPlayers, int selectedPlayer, Point p) {
    char newPlayerName[nameLenght*2+1];   // Új játékos hozzáadásakor ebbe kerül a név
    Player *editablePlayer;
    lib_ClearScrBellow();
    // Alcím kiírása jáékos név kipontozása
    printfc(strNewPlayerTitle, p.x - stringlenght(strNewPlayerTitle)/2, p.y, baseForeColor);
    printfc("Név (max 20 karakter):", p.x-11, p.y+1, baseForeColor);
    printfc("____________________", p.x-10, p.y+2, baseForeColor);
    if (*state == editPlayer){ // Játékosnév változtatásához eredeti név kiírása
         editablePlayer = player_GetSelectedPlayer(*playerListHead, selectedPlayer);
        printfc(strEditPlayerTitle, p.x-13, p.y, baseForeColor);
        printfc(editablePlayer->name, p.x - 10, p.y + 2, baseForeColor);
    }
    econio_gotoxy(p.x-10, p.y+2);
    // Bemenet várása a felhasználótól
    fgets(newPlayerName, nameLenght*2+1, stdin);

    // Bemenet kiértékelése
    if (!isBlankString(newPlayerName)){
        // Új játékosnév lerövidítése
        newPlayerName[strlen(newPlayerName)-1] = '\0';
        int numOfDisplayedCharacter = stringlenght(newPlayerName);
        int maxDisplayableCharacters= stringlenghtMax(newPlayerName, nameLenght);
        int realLenght = numOfDisplayedCharacter <= nameLenght ? strlen(newPlayerName) : maxDisplayableCharacters;
        newPlayerName[realLenght] = '\0';
        // TODO: Képernyőtörlés hülye felhasználók miatt
        // Player Listába beolvasás fájlból
        player_ReadTxtFile(playerListHead, numOfPlayers); // Új játékoshoz beolvasás fájlból

        // Játékosnév tesztelése, hogy létezik e már az adatbázisban
        int exists = player_GetIndexOfPlayer(*playerListHead, newPlayerName);
        if (exists == -1) {
            // Új játékos hozzáaadása a láncolt listához
            if (*state == editPlayer){
                player_AddPlayerInOrder(player_MakePlayer(newPlayerName, editablePlayer->numOfCompletedLevels, (Statistics*) editablePlayer->levelStats), playerListHead, numOfPlayers);
                player_RemovePlayer(editablePlayer, playerListHead, numOfPlayers);
            }
            else{
                player_AddPlayerInOrder(player_MakePlayer(newPlayerName,0,NULL), playerListHead, numOfPlayers);
            }
            // Player Láncoltl ista fájlba mentése
            player_WriteTxtFile(*playerListHead, *numOfPlayers);
            printfc("ELMENTVE", p.x- stringlenght("ELMENTVE")/2, p.y+4, baseForeColor);
        }
        else{
            printfc("LÉTEZIK MÁR ILYEN NEVŰ JÁTÉKOS!", p.x- stringlenght("LÉTEZIK MÁR ILYEN NEVŰ JÁTÉKOS!")/2, p.y+4, baseForeColor);
        }
    }
    else
        printfc("MÉGSE", p.x- stringlenght("MÉGSE")/2, p.y+4, baseForeColor);
    econio_sleep(waitsec);
    econio_clrscr();
    menu_PrintTitle();
    if (*state == newPlayer) *state = mainMenu;
    else if (*state == editPlayer) *state = chosePlayer;
}

static void menu_PrintPlayerSubMenu(bool *displayFirst, Player **playerListHead, int *numOfPlayers, Player **currentPlayer, int selectedPlayer, Point p ) {
    if (*displayFirst){
        *displayFirst = false;
        lib_ClearScrBellow();
        // Alcím kiírása
        printfc(strChosePlayerTitle, p.x - stringlenght(strChosePlayerTitle)/2, p.y, baseForeColor);
        // Player Listába beolvasás fájlból
        player_ReadTxtFile(playerListHead, numOfPlayers); // Játékosok kiiratásához beolvasás
    }
    // Player Lista kiiratása a képernyőre
    player_PrintPlayerList(*playerListHead, selectedPlayer, (Point) {p.x, p.y + 1} );
    // Kijelölt játékos a currentPlayerbe rakása, hogy a játék indítható legyen
    *currentPlayer = player_GetSelectedPlayer(*playerListHead, selectedPlayer);
}

static void menu_PrintRankList(bool *displayFirst, Player **playerListHead, int *numOfPlayers, Point p, int maxDisplayLvls){
    if (*displayFirst) {
        *displayFirst = false;
        lib_ClearScrBellow();
        player_ReadTxtFile(playerListHead, numOfPlayers); // Rangsorhoz beolvasás fájlból
        printfc(strRankListTitle,p.x-stringlenght(strRankListTitle)/2, p.y, baseForeColor);
    }
    if (*numOfPlayers > 0) {
        // Segéd változók
        Player *mover = *playerListHead;
        int *spaces = (int*) malloc(*numOfPlayers * sizeof(int));
        int lenght;
        int firstindent = 7; // Szint->║ Név ║
        int totalLenght = firstindent;
        int playerindex = 0;
//        int totalPlayerDisplay = 0;
//        int horizontalPageCounter = 0;
        int maxLvls = 0;
        // Nevek hosszának és max szintek megállapítása
        while (mover != NULL){
            lenght = (int) stringlenght(mover->name); // Játékosnév karakterhossza
            totalLenght += lenght + 3; // Hosssz + Szóközök hozzáadása
            spaces[playerindex++] = lenght;
//            maxLvls = maxLvls < mover->numOfCompletedLevels ? mover->numOfCompletedLevels : maxLvls;
            mover = (Player *) mover->next;
        }
//        playerindex = 0;
//        page.x = page.x > maxSize.width / totalLenght ? maxSize.width / totalLenght : page.x;
//        page.y = page.y > maxDisplayLvls / maxLvls ? maxDisplayLvls / maxLvls : page.y;
//        // Oldal kiválasztása
//        horizontalPageCounter =
//        totalPlayerDisplay = playerindex;

        // Segédváltozók visszaállítása
        p = (Point) {p.x - totalLenght/2, p.y};
        mover = *playerListHead;
        playerindex = 0;
        int line = 1, maxline = 0;
        printfc(" Szint ┃ ",p.x, p.y + line++, baseForeColor);
        printfc("━━━━━━━╋ ",p.x, p.y + line, baseForeColor);
        char text[nameLenght+5],
                helper[nameLenght+5];
        int indent = firstindent + 1;

        // Player listán végigmegyünk
        while (mover != NULL){
            line = 1;
            // Fejléc kiiratása
            sprintf(text, "┃ %s ", mover->name);
            printfc(text, p.x+indent-1, p.y+line++, baseForeColor);
            for(int i = 0; i < spaces[playerindex] + 2; i++) printfc("━", p.x + indent + i, p.y + line, baseForeColor);
            if (playerindex < *numOfPlayers - 1) printfc("╋", p.x + indent + + spaces[playerindex] + 2, p.y + line, baseForeColor);
            line++;
            // Szint számának kiiratása
            int lvl = 1;
            for (Statistics* stat = (Statistics *) mover->levelStats; stat != NULL /*&& lvl <= maxDisplayLvls*/; stat = (Statistics *) stat->next){
                // Szint sorszámának kiírása
                sprintf(text, "  %3d. ┃", lvl);
                printfc(text, p.x, p.y+line, baseForeColor);
                // Lépésszám kiratása
                strcpy(text, "%");
                sprintf(helper, "%d", spaces[playerindex]);
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
        line = 1;
        indent = firstindent + 1;
        for(int j = 0; j < *numOfPlayers - 1; j++){
            indent += spaces[j] + 3;
            for(int k = 2; k < maxline-1; k++) printfc("┃", p.x+indent-1, p.y+line+k, baseForeColor);
        }
        free(spaces);
    }
    else{
        printfc("Nincs egy játékos sem még.",p.x - (int)stringlenght("Nincs egy játékos sem még.")/2, p.y, baseForeColor);
    }
}

static void menu_ResetMenuVars(bool *displayFirst, int *option, int *selectedPlayer) {
    *displayFirst = true;
    *option = 0;
    *selectedPlayer = 0;
}

void menu_PrintNavControls(bool displayFirst, State state) {
    if (displayFirst) {
        Point p = {6, maxDisplayLines+12};
        int i = 0;
        lib_ClearScreenSection(0,p.y,maxSize.width,maxSize.height, COL_RESET);
        switch (state) {
            case mainMenu:
                printfc("[↑] : Feljebb mozgás", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[↓] : Lejjebb mozgás", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[↵] : Kiválasztás", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[Esc] : Kilépés", p.x - 6, p.y + i++, COL_LIGHTBLUE);
                break;
            case newPlayer:
            case editPlayer:
                printfc("[↵] : Mentés / Vissza", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                break;
            case chosePlayer:
                printfc("[↑] : Feljebb mozgás", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[↓] : Lejjebb mozgás", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[↵] : Kiválasztás", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[E] : Szerkesztés", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[D] : Törlés", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[Esc] : Vissza", p.x - 6, p.y + i++, COL_LIGHTBLUE);
                break;
            case rankList:
//                printfc("[↑] : Felfele lapoz", p.x - 4, p.y + i++, COL_LIGHTBLUE);
//                printfc("[↓] : Lefele lapoz", p.x - 4, p.y + i++, COL_LIGHTBLUE);
//                printfc("[→] : Jobbra lapoz", p.x - 4, p.y + i++, COL_LIGHTBLUE);
//                printfc("[←] : Balra lapoz", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[Esc] : Vissza", p.x - 6, p.y + i++, COL_LIGHTBLUE);
                break;
            case exitApp:
            case deletePlayer:
            case exitGame:
                printfc("[→] : Igen választása", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[←] : Nem választása", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[Esc] : Vissza", p.x - 6, p.y + i++, COL_LIGHTBLUE);
                break;
            case game:
                printfc("[↑] : Felfele lép", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[↓] : Lefele lép", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[→] : Jobbra lép", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[←] : Balra lép", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[V] : Visszalépés", p.x - 4, p.y + i++, baseForeColor);
                printfc("[R] : Szint újrakezése", p.x - 4, p.y + i++, baseForeColor);
                printfc("[Esc] : Kilépés a játékból", p.x - 6, p.y + i++, COL_LIGHTBLUE);

            default: break;
        }
    }
}
