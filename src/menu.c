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
// Menü globális változói
int keyM;              // Lenyomott billentyű kódja
int optionM;           // Aktuálisan kiválasztott opció
int prevOption;       // Előző választott opció
int selectedPlayer;   // Az aktuálisan kiválasztott játékos indexe a láncolt listában és a menüben
bool displayFirstM;    // Igaz, ha egy menüponba először megyünk bele, Hamis, ha már a többedjére
bool runMenu;         // Amíg igaz, addig fut a menü ciklusa, hamisnál leáll a menü, és kilép a programból
State state;          // A menürendszer aktuális állapotát tárolja



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
}
static void menu_PrintNavControls() {
    if (displayFirstM) {
        Point p = {6, maxDisplayLines+12};
        int i = 0;
        lib_ClearScreenSection(0,p.y,maxSize.width,maxSize.height, COL_RESET);
        switch (state) {
            case mainMenu:
                printfc("[↑] : Feljebb mozgás", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[↓] : Lejjebb mozgás", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[↵] : Kiválasztás", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[Esc] : Kilépés", p.x - 6, p.y + i, COL_LIGHTBLUE);
                break;
            case newPlayer:
            case editPlayer:
                printfc("[↵] : Mentés / Vissza", p.x - 4, p.y + i, COL_LIGHTBLUE);
                break;
            case chosePlayer:
                printfc("[↑] : Feljebb mozgás", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[↓] : Lejjebb mozgás", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[↵] : Játék indítása", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[E] : Szerkesztés", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[D] : Törlés", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[Esc] : Vissza", p.x - 6, p.y + i, COL_LIGHTBLUE);
                break;
            case rankList:
//                printfc("[↑] : Felfele lapoz", p.x - 4, p.y + i++, COL_LIGHTBLUE);
//                printfc("[↓] : Lefele lapoz", p.x - 4, p.y + i++, COL_LIGHTBLUE);
//                printfc("[→] : Jobbra lapoz", p.x - 4, p.y + i++, COL_LIGHTBLUE);
//                printfc("[←] : Balra lapoz", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[Esc] : Vissza", p.x - 6, p.y + i, COL_LIGHTBLUE);
                break;
            case exitApp:
            case deletePlayer:
            case exitGame:
                printfc("[→] : Igen választása", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[←] : Nem választása", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[Esc] : Vissza", p.x - 6, p.y + i, COL_LIGHTBLUE);
                break;
            case game:
                printfc("[↑] : Felfele lép", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[↓] : Lefele lép", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[→] : Jobbra lép", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[←] : Balra lép", p.x - 4, p.y + i++, COL_LIGHTBLUE);
                printfc("[V] : Visszalépés", p.x - 4, p.y + i++, baseForeColor);
                printfc("[R] : Szint újrakezése", p.x - 4, p.y + i++, baseForeColor);
                printfc("[Esc] : Kilépés a játékból", p.x - 6, p.y + i, COL_LIGHTBLUE);

            default: break;
        }
    }
}
static void menu_PrintExitWindow(Point p) {
    // Ha fut a menü
    if (runMenu){
        lib_WarningWindow("BIZTOSAN KILÉPSZ?", p, &displayFirstM, optionM, COL_RED, COL_WHITE, COL_LIGHTRED);
    }
    else {
        // Leíállítás kiírása
        econio_clrscr();
        printfc("LEÁLLÍTÁS", 33, 10, COL_RED);
        econio_sleep(waitsec);
    }
}
static void menu_PrintMainMenu(Point p) {
    if (displayFirstM){
        displayFirstM = false;
        lib_ClearScrBellow();
        printfc(strNewPlayer, p.x - (int) utf8_strlen(strNewPlayer) / 2, p.y, baseForeColor); // 0
        printfc(strChosePlayer, p.x - (int) utf8_strlen(strChosePlayer) / 2, p.y + 1, baseForeColor); // 0
        printfc(strRankList, p.x - (int) utf8_strlen(strRankList) / 2, p.y + 2, baseForeColor); // 0
        printfc(strExitApp, p.x - (int) utf8_strlen(strExitApp) / 2, p.y + 3, baseForeColor); // 0
    }
    // Előző opcó deaktiválása
    switch (prevOption) {
        case 0:
            printfc(strNewPlayer, p.x - (int) utf8_strlen(strNewPlayer) / 2, p.y, baseForeColor); // 0
            break;
        case 1:
            printfc(strChosePlayer, p.x - (int) utf8_strlen(strChosePlayer) / 2, p.y + prevOption, baseForeColor); // 0
            break;
        case 2:
            printfc(strRankList, p.x - (int) utf8_strlen(strRankList) / 2, p.y + prevOption, baseForeColor); // 0
            break;
        case 3:
            printfc(strExitApp, p.x - (int) utf8_strlen(strExitApp) / 2, p.y + prevOption, baseForeColor); // 0
            break;
        default: break;
    }
    // Aktuális opció kiemelésa
    switch (optionM){
        case 0:
            printfbc(strNewPlayer,p.x - (int) utf8_strlen(strNewPlayer) / 2, p.y, activeForeColor, activeBgColor); // 0
            break;
        case 1:
            printfbc(strChosePlayer, p.x - (int) utf8_strlen(strChosePlayer) / 2, p.y + optionM, activeForeColor, activeBgColor); // 0
            break;
        case 2:
            printfbc(strRankList, p.x - (int) utf8_strlen(strRankList) / 2, p.y + optionM, activeForeColor, activeBgColor); // 0
            break;
        case 3:
            printfbc(strExitApp, p.x - (int) utf8_strlen(strExitApp) / 2, p.y + optionM, activeForeColor, activeBgColor); // 0
            break;
        default: break;
    }
}
static void menu_PrintNewPlayerSubMenu(Player **playerListHead, int *numOfPlayers, Point p) {
    char newPlayerName[nameLenght*2+1];   // Új játékos hozzáadásakor ebbe kerül a név
    Player *editablePlayer;
    lib_ClearScrBellow();
    // Alcím kiírása jáékos név kipontozása
    printfc(strNewPlayerTitle, p.x - utf8_strlen(strNewPlayerTitle) / 2, p.y, baseForeColor);
    printfc("Név (max 20 karakter):", p.x-11, p.y+1, baseForeColor);
    printfc("____________________", p.x-10, p.y+2, baseForeColor);
    if (state == editPlayer){ // Játékosnév változtatásához eredeti név kiírása
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
        int numOfDisplayedCharacter = utf8_strlen(newPlayerName);
        int maxDisplayableCharactersBytes= stringlenghtMax(newPlayerName, nameLenght);
        int realByteLenght = numOfDisplayedCharacter <= nameLenght ? (int)strlen(newPlayerName) : maxDisplayableCharactersBytes;
        newPlayerName[realByteLenght] = '\0';
        // Player Listába beolvasás fájlból
        player_ReadTxtFile(playerListHead, numOfPlayers); // Új játékoshoz beolvasás fájlból

        // Játékosnév tesztelése, hogy létezik e már az adatbázisban
        int exists = player_GetIndexOfPlayer(*playerListHead, newPlayerName);
        if (exists == -1) {
            // Új játékos hozzáaadása a láncolt listához
            if (state == editPlayer){
                player_AddPlayerInOrder(player_MakePlayer(newPlayerName, editablePlayer->numOfCompletedLevels, (Statistics*) editablePlayer->levelStats), playerListHead, numOfPlayers);
                player_RemovePlayer(editablePlayer, playerListHead, numOfPlayers);
            }
            else{
                player_AddPlayerInOrder(player_MakePlayer(newPlayerName,0,NULL), playerListHead, numOfPlayers);
            }
            // Player Láncoltl ista fájlba mentése
            player_WriteTxtFile(*playerListHead);
            printfc("ELMENTVE", p.x - utf8_strlen("ELMENTVE") / 2, p.y + 4, baseForeColor);
        }
        else{
            printfc("LÉTEZIK MÁR ILYEN NEVŰ JÁTÉKOS!", p.x - utf8_strlen("LÉTEZIK MÁR ILYEN NEVŰ JÁTÉKOS!") / 2, p.y + 4, baseForeColor);
        }
    }
    else
        printfc("MÉGSE", p.x - utf8_strlen("MÉGSE") / 2, p.y + 4, baseForeColor);
    econio_sleep(waitsec);
    system("cls");
    menu_PrintTitle();
    if (state == newPlayer) state = mainMenu;
    else if (state == editPlayer) state = chosePlayer;
}
static void menu_PrintPlayerSubMenu(Player **playerListHead, Player **currentPlayer, int *numOfPlayers, Point p ) {
    if (displayFirstM){
        displayFirstM = false;
        lib_ClearScrBellow();
        // Alcím kiírása
        printfc(strChosePlayerTitle, p.x - utf8_strlen(strChosePlayerTitle) / 2, p.y, baseForeColor);
        // Player Listába beolvasás fájlból
        player_ReadTxtFile(playerListHead, numOfPlayers); // Játékosok kiiratásához beolvasás
    }
    // Player Lista kiiratása a képernyőre
    player_PrintPlayerList(*playerListHead, selectedPlayer, (Point) {p.x, p.y + 1} );
    // Kijelölt játékos a currentPlayerbe rakása, hogy a játék indítható legyen
    *currentPlayer = player_GetSelectedPlayer(*playerListHead, selectedPlayer);
}
static void menu_PrintRankList( Player **playerListHead, int *numOfPlayers, Point p){
    if (displayFirstM) {
        displayFirstM = false;
        lib_ClearScrBellow();
        player_ReadTxtFile(playerListHead, numOfPlayers); // Rangsorhoz beolvasás fájlból
        printfc(strRankListTitle, p.x - utf8_strlen(strRankListTitle) / 2, p.y, baseForeColor);
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
//        int maxLvls = 0;
        // Nevek hosszának és max szintek megállapítása
        while (mover != NULL){
            lenght = (int) utf8_strlen(mover->name); // Játékosnév karakterhossza
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
        char text[nameLenght*2+6],
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
        printfc("Nincs egy játékos sem még.",p.x - (int) utf8_strlen("Nincs egy játékos sem még.") / 2, p.y, baseForeColor);
    }
}
static void menu_PrintWinGame(Point p) {
    lib_ClearScrBellow();
    printfc("TELJESÍTETTED AZ ÖSSZES SZINTEN!", p.x-16, p.y+3,COL_LIGHTGREEN);
    printfc("GRATULÁLOK!", p.x-6, p.y+4,COL_LIGHTYELLOW);
    econio_sleep(2);
    state = chosePlayer;
    displayFirstM = true;
}
static void menu_ResetMenuVars() {
    displayFirstM = true;
    optionM = 0;
    selectedPlayer = 0;
}
static void menu_KeyPress(Player *currentPlayer, Player **playerListHead,  int *numOfPlayers, char **levelList, int numOfLevels){
    switch (keyM){
        case KEY_ESCAPE:
        case KEY_BACKSPACE:
            if (state == mainMenu) state = exitApp;
            else if (state == deletePlayer) state = chosePlayer;
            else if (state == rankList){
                state = mainMenu;
                system("cls");
                menu_PrintTitle();
            }
            else state = mainMenu;
            // Reset Menü
            menu_ResetMenuVars();
            break;
        case KEY_ENTER:
            switch (state) {
                case exitApp:
                    if (optionM == 1)
                        runMenu = false; // Kilépés a menüből
                    else{
                        // Viszzalépés a főmenübe
                        state = mainMenu;
                        menu_ResetMenuVars();
                    }
                    break;
                case mainMenu:
                    state = mainMenu + 1 + optionM; // Főmenüből kijelölt opcióbaváltás
                    menu_ResetMenuVars();
                    break;
                case chosePlayer:
                    if (currentPlayer != NULL){
                        state = game;
                        displayFirstM = true;
                        menu_PrintNavControls();
                        // Játék indítása
                        while(game_Init(currentPlayer, levelList) && currentPlayer->numOfCompletedLevels <= numOfLevels) {
                            // Nem kell ide semmi, addig fut, amíg a játékból ki nem lépnek
                            // vagy nem teljesíti az összes szintet a játékos
                        }
                        if (currentPlayer->numOfCompletedLevels >= numOfLevels) state = winGame;
                        else state = chosePlayer;
                        player_WriteTxtFile(*playerListHead);
                        menu_ResetMenuVars();
                    }
                    break;
                case deletePlayer:
                    if (optionM == 1){
                        player_RemovePlayer(currentPlayer,playerListHead, numOfPlayers);
                        player_WriteTxtFile(*playerListHead);
                    }
                    state = chosePlayer;
                    menu_ResetMenuVars();
                    break;
                default: break;
            }
            break;
            // Navigációk a menüben
        case KEY_RIGHT:
            if (state >= exitApp && optionM == 0)
                optionM = 1; // IGEN/NEM választása a kilépésnél
            //else if (*state == rankList) page->y++; // Dicsőséglistánál lapozás jobbra
            break;
        case KEY_LEFT:
            if (state >= exitApp && optionM == 1)
                optionM = 0; // IGEN/NEM választása a kilépésnél
            //else if (*state == rankList && page->y > 0) page->y--; // Dicsőséglistánál lapozás balra
            break;
        case KEY_UP:
            if (state == mainMenu && optionM > 0)
                (optionM)--; // Felfele lépés a főmenüben
            else if (state == chosePlayer && selectedPlayer > 0)
                (selectedPlayer)--; // Felfele lépés a játékosneveken
            //else if (*state == rankList && page->x > 0) page->x--; // Dicsőséglistánál lapozás felfele
            break;
        case KEY_DOWN:
            if (state == mainMenu && optionM < exitApp - 1)
                (optionM)++; // Lefele lépés a főmenüben
            else if (state == chosePlayer && selectedPlayer < (*numOfPlayers)-1)
                (selectedPlayer)++; // Lefele lépés a játékosneveken
            //else if (*state == rankList) page->x++; // Dicsőséglistánál lapozás lefele
            break;
        case 'E':
        case 'e':
        case KEY_F2: // Játékos név szerkesztése
            if (state == chosePlayer){
                state = editPlayer;
                displayFirstM = true;
            }
            break;
        case 'D':
        case 'd':
        case KEY_DELETE:// Játékos eltávolítása
            if (state == chosePlayer){
                state = deletePlayer;
                menu_ResetMenuVars();
                displayFirstM = true;
            }
            break;
        default: break;
    }
}
static void menu_EvaluateState(Player **playerListHead, int *numOfPlayers, Player **currentPlayer, Point p, int *linesPrinted){
    menu_PrintNavControls();
    switch (state) {
        case exitApp: // Kilépési ablak
            menu_PrintExitWindow(p);
            *linesPrinted = 6;
            break;
        case mainMenu: // Főmenü kiiratása
            menu_PrintMainMenu(p);
            *linesPrinted = 4;
            break;
        case newPlayer: // Új játékos felvétele
            menu_PrintNewPlayerSubMenu(playerListHead, numOfPlayers, p);
            *linesPrinted = 4;
            keyM = -1;
            break;
        case chosePlayer: // Játékos választás kiiratása
            menu_PrintPlayerSubMenu(playerListHead, currentPlayer, numOfPlayers, p);
            *linesPrinted = *numOfPlayers + 1;
            break;
        case deletePlayer:
            lib_WarningWindow("BIZTOSAN TÖRLÖD?", p, &displayFirstM, optionM, baseForeColor, activeForeColor, activeBgColor);
            *linesPrinted = 6;
            break;
        case editPlayer:
            menu_PrintNewPlayerSubMenu(playerListHead, numOfPlayers, p);
            *linesPrinted = 4;
            displayFirstM = true;
            keyM = -1;
            break;
        case rankList: // Dicsőséglista kiiratása
            menu_PrintRankList( playerListHead, numOfPlayers, p);
            *linesPrinted = maxDisplayLines;
            break;
        case winGame:
            menu_PrintWinGame(p);
            keyM = -1;
            break;
        default: break;
    }
}

void menu_MainScreen(){

    // Konstansok a kiiratáshoz
    const int center = maxSize.width/2; // Képernyő közepe a cím szerint

    Point p;                        // A kiiratás középpontja a cím alatt
    // Menü Változók
    keyM = -1;                   // Lenyomott billentyű kódja
    optionM = 0;                 // Aktuálisan kiválasztott opció
    prevOption = 0;                 // Előző választott opció
    selectedPlayer = 0;         // Az aktuálisan kiválasztott játékos indexe a láncolt listában és a menüben
    displayFirstM = true;       // Igaz, ha egy menüponba először megyünk bele, Hamis, ha már a többedjére
    runMenu = true;            // Amíg igaz, addig fut a menü ciklusa, hamisnál leáll a menü, és kilép a programból
    state = mainMenu;         // A menürendszer aktuális állapotát tárolja

    // Dinamikus tömbök és hozzátartozó adatok
    Player *currentPlayer = NULL;   // Aktuálisan kiválasztott játékost tároló pointer
    char **levelList = NULL;        // A szintek fájlneveit tartalmazza a ./levels/ mappából, dinamikus string tömb
    int numOfLevels = 0;            // A beolvasott szintek száma a ./levels/ mappából, levelList tömb hossza
    Player *playerListHead = NULL;  // A Játékosok adatait tároló dinamikus láncolt lista
    int numOfPlayers = 0;           // A Játékosok száma a playerListHead lácnolt listába

    int linesPrinted = 0;           // A konzolra írt sorok száma egyes kiiratásokor
    // Főcím kiiratása "SOKOBAN"
    menu_PrintTitle();
    // Szintek mappájának beolvasása
    level_ReadDirectory("levels", &levelList, &numOfLevels);
    econio_rawmode(); // Billentyűérzékelés bekapcsolása

    // == MENÜ FUTTATÁSA ==
    while(runMenu){
        p = (Point) {center, 9};
        // Előző kijelölés eltárolása;
        prevOption = optionM;
        // Lenyomott billentyű kiértékelése
        menu_KeyPress(currentPlayer, &playerListHead, &numOfPlayers, levelList, numOfLevels);
        // Képernyőre írás menüpont alapján
        menu_EvaluateState(&playerListHead, &numOfPlayers, &currentPlayer, p, &linesPrinted);
        // Billentyűlenyomásra vár, ha fut a menü
        if (runMenu && !displayFirstM) keyM = econio_getch();
    }
    // Játékosok adatainak mentése
    player_WriteTxtFile(playerListHead);
    // Játékosok listájának felszabadítása
    player_FreePlayerList(&playerListHead); // Player lista felszabadítása
    // Lefoglalt levelLista felszabadítása
    level_FreeLevelList(&levelList, &numOfLevels);

    debugmalloc_log_file("debugmalloc.txt");
}