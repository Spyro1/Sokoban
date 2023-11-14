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

#ifdef _WIN32
#include <windows.h>

#endif

void MainScreen(){

    // Konstansok a kiiratáshoz
    const Size maxSize = {72,30};
    const int center = maxSize.width/2; // Képernyő közepe a cím szerint

    Point p = {center, 9}; // A kiiratás középpontja a cím alatt

    // Menü Változók
    Player *currentPlayer = NULL;   // Aktuálisan kiválasztott játékost tároló pointer
    int selectedLevel = 0;          // Kiválasztott szint
    int key = -1;                   // Lenyomott billentyű kódja
    bool displayFirst = true;       // Igaz, ha egy menüponba először megyünk bele, Hamis, ha már a többedjére
    bool runMenu = true;            // Amíg igaz, addig fut a menü ciklusa, hamisnál leáll a menü, és kilép a programból
    int option = 0;                 // Aktuálisan kiválasztott opció
    int prevOption = 0;             // Előző választott opció
    int numOfPlayers = 0;           // A Játékosok száma a playerListHead lácnolt listába
    int selectedPlayer = 0;         // Az aktuálisan kiválasztott játékos indexe a láncolt listában és a menüben
    Player *playerListHead = NULL;  // A Játékosok adatait tároló dinamikus láncolt lista
    char **levelList = NULL;        // A szintek fájlneveit tartalmazza a ./levels/ mappából, dinamikus string tömb
    int numOfLevels = 0;            // A beolvasott szintek száma a ./levels/ mappából, levelList tömb hossza

    int linesPrinted = 0;               // A konzolra írt sorok száma egyes kiiratásokor
    State state = mainMenu;    // A menürendszer aktuális állapotát tárolja

    // Főcím kiiratása "SOKOBAN"
    PrintTitle();
    // Szint Mappa beolvasása
    ReadDirectoryLevelNames("./levels/", &levelList, &numOfLevels);

    econio_rawmode(); // Billentyűérzékelés bekapcsolása

    // == MENÜ FUTTATÁSA ==
    while(runMenu){
        p = (Point) {center, 9};
        prevOption = option;
        // Lenyomott billentyű kiértékelése
        KeyPress(key, &state, &displayFirst, &option, &selectedPlayer, &runMenu, currentPlayer, playerListHead, &numOfPlayers, levelList);

        // Képernyőre írás választott mód szerint
        switch (state) {
            case exitApp: // Kilépési ablak
                PrintExitWindow(runMenu, &displayFirst, option, p);
                linesPrinted = 6;
                break;
            case mainMenu: // Főmenü kiiratása
                PrintMainMenu(&displayFirst, option, prevOption, p);
                linesPrinted = 4;
                break;
            case newPlayer: // Új játékos felvétele
                PrintNewPlayerSubMenu(&state, &playerListHead, &numOfPlayers, selectedPlayer, p);
                linesPrinted = 4;
                key = -1;
                break;
            case chosePlayer: // Játékos választás kiiratása
                PrintPlayerSubMenu(&displayFirst, &playerListHead, &numOfPlayers, &currentPlayer, selectedPlayer, p);
                linesPrinted = numOfPlayers + 1;
                break;
            case deletePlayer:
                WarningWindow("BIZTOSAN TÖRLÖD?", p, option, &displayFirst, baseForeColor, activeForeColor, activeBgColor);
                linesPrinted = 6;
                break;
            case editPlayer:
                PrintNewPlayerSubMenu(&state, &playerListHead, &numOfPlayers, selectedPlayer, p);
                linesPrinted = 4;
                break;
            case rankList: // Dicsőséglista kiiratása
                PrintRankList(&displayFirst, playerListHead, &numOfPlayers, p, maxDisplayLines);
                linesPrinted = maxDisplayLines;
                break;
            default: break;
        }
//        PrintNavControls(state, p, maxSize);
        if (runMenu && !displayFirst) key = econio_getch(); // Billentyűlenyomásra vár, ha fut a menü
    }
    // Játékosok adatainak mentése
    player_WriteTxtFile(playerListHead, numOfPlayers);
    // Játékosok listájának felszabadítása
    player_FreePlayerList(&playerListHead); // Player lista felszabadítása
    // Lefoglalt levelLista felszabadítása
    FreeLevelList(&levelList, &numOfLevels);
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
//    printfc(             "↑    ⌫", 16, 24,COL_LIGHTCYAN);
//    printfc("Navigálás: ← ↓ →  ↲", 3 , 25,COL_LIGHTCYAN);
}

void KeyPress(int key, State *state, bool *displayFirst, int *option, int *selectedPlayer, bool *runMenu, Player *currentPlayer, Player *playerListHead, int *numOfPlayers, char **levelList){
    switch (key){
        case KEY_ESCAPE:
        case KEY_BACKSPACE:
            if (*state == mainMenu) *state = exitApp;
            else if (*state == deletePlayer) *state = chosePlayer;
            else *state = mainMenu;
            // Reset Menü
            ResetMenuVars(displayFirst, option, selectedPlayer);
            break;
        case KEY_ENTER:
            switch (*state) {
                case exitApp:
                    if (*option == 1)
                        *runMenu = false; // Kilépés a menüből
                    else{
                        // Viszzalépés a főmenübe
                        *state = mainMenu;
                        ResetMenuVars(displayFirst, option, selectedPlayer);
                    }
                    break;
                case mainMenu:
                    *state = mainMenu + 1 + *option; // Főmenüből kijelölt opcióbaváltás
                    ResetMenuVars(displayFirst, option, selectedPlayer);
                    break;
                case chosePlayer:
                    if (currentPlayer != NULL){
                        // Játék indítása
                        while(Init(currentPlayer, levelList)) {
                            // Nem kell ide semmi, addig fut, amíg a játékból ki nem lépnek
                            // vagy nem teljesíti az összes szintet a játékos
                        }
                        player_WriteTxtFile(playerListHead, *numOfPlayers);
                        ResetMenuVars(displayFirst, option, selectedPlayer);
                    }
                    break;
                case deletePlayer:
                    if (*option == 1){
                        player_RemovePlayer(currentPlayer,&playerListHead, numOfPlayers);
                        player_WriteTxtFile(playerListHead, *numOfPlayers);
                    }
                    *state = chosePlayer;
                    ResetMenuVars(displayFirst, option, selectedPlayer);
                    break;
                default: break;
            }
            break;
            // Navigációk a menüben
        case KEY_RIGHT:
            if (*state >= exitApp && *option == 0)
                *option = 1; // IGEN/NEM választása a kilépésnél
            break;
        case KEY_LEFT:
            if (*state >= exitApp && *option == 1)
                *option = 0; // IGEN/NEM választása a kilépésnél
            break;
        case KEY_UP:
            if (*state == mainMenu && *option > 0)
                (*option)--; // Felfele lépés a főmenüben
            else if (*state == chosePlayer && *selectedPlayer > 0)
                (*selectedPlayer)--; // Felfele lépés a játékosneveken
            break;
        case KEY_DOWN:
            if (*state == mainMenu && *option < exitApp-1)
                (*option)++; // Lefele lépés a főmenüben
            else if (*state == chosePlayer && *selectedPlayer < (*numOfPlayers)-1)
                (*selectedPlayer)++; // Lefele lépés a játékosneveken
            break;
        case 'E':
        case 'e':
        case KEY_F2:
            if (*state == chosePlayer){
                *state = editPlayer;
            }
            // Játékos név szerkesztlse
            break;
        case 'D':
        case 'd':
        case KEY_DELETE:
            if (*state == chosePlayer){
                *state = deletePlayer;
                ResetMenuVars(displayFirst, option, selectedPlayer);
                // Játékos eltávolítása
            }
            break;
        default: break;
    }
}

void PrintExitWindow(bool runMenu, bool *displayFirst, int option, Point p) {
    // Ha fut a menü
    if (runMenu){
        WarningWindow("BIZTOSAN KILÉPSZ?", p, option, displayFirst, COL_RED, COL_WHITE, COL_LIGHTRED);
    }
    else {
        // Leíállítás kiírása
        econio_clrscr();
        printfc("LEÁLLÍTÁS", 33, 10, COL_RED);
        Sleep(waitms);
    }
}

void PrintMainMenu(bool *displayFirst, int option, int prevOption, Point p) {
    if (*displayFirst){
        *displayFirst = false;
        ClearScrBellow();
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

void PrintNewPlayerSubMenu(State *state, Player **playerListHead, int *numOfPlayers, int selectedPlayer, Point p) {
    char newPlayerName[nameLenght*2];   // Új játékos hozzáadásakor ebbe kerül a név
    Player *editablePlayer;
    ClearScrBellow();
    // Alcím kiírása jáékos név kipontozása
    printfc("Név (max 20 karakter):", p.x-11, p.y, baseForeColor);
    printfc("____________________", p.x-10, p.y+1, baseForeColor);
    if (*state == editPlayer){ // Játékosnév változtatásához eredeti név kiírása
         editablePlayer = player_GetSelectedPlayer(*playerListHead, selectedPlayer);
        printfc(editablePlayer->name, p.x - 10, p.y + 1, baseForeColor);
    }
    econio_gotoxy(p.x-10, p.y+1);
    // Bemenet várása a felhasználótól
    fgets(newPlayerName, nameLenght*2+1, stdin);

    // Ha nem egy üres sort írt be, akkor eltároljuk az új játékost
    // Ezen még kell finomítani a különböző hibaesetekre, pl üres sor
    if (newPlayerName[0] != '\n'){
        // Új játékosnév lerövidítése
        newPlayerName[strlen(newPlayerName)-1] = '\0';
        int numOfDisplayedCharacter = stringlenght(newPlayerName);
        int maxDisplayableCharacters= stringlenghtMax(newPlayerName, nameLenght);
        int realLenght = numOfDisplayedCharacter <= nameLenght ? strlen(newPlayerName) : maxDisplayableCharacters;
        newPlayerName[realLenght] = '\0';

        // Player Listába beolvasás fájlból
        player_ReadTxtFile(playerListHead, numOfPlayers); // Új játékoshoz beolvasás fájlból

        // Játékosnév tesztelése, hogy létezik e már az adatbázisban
        int exists = player_GetIndexOfPlayer(*playerListHead, newPlayerName);
        if (exists == -1) {
            // Új játékos hozzáaadása a láncolt listához
            if (*state == editPlayer && strcmp(editablePlayer->name, newPlayerName) != 0){
                player_AddPlayerInOrder(player_MakePlayer(newPlayerName, editablePlayer->numOfCompletedLevels, (Statistics*) editablePlayer->levelStats), playerListHead, numOfPlayers);
                player_RemovePlayer(editablePlayer, playerListHead, numOfPlayers);
            }
            else{
                player_AddPlayerInOrder(player_MakePlayer(newPlayerName,0,NULL), playerListHead, numOfPlayers);
            }
            // Player Láncoltl ista fájlba mentése
            player_WriteTxtFile(*playerListHead, *numOfPlayers);
            printfc("ELMENTVE", p.x- stringlenght("ELMENTVE")/2, p.y+3, baseForeColor);
        }
        else{
            printfc("LÉTEZIK MÁR ILYEN NEVŰ JÁTÉKOS!", p.x- stringlenght("LÉTEZIK MÁR ILYEN NEVŰ JÁTÉKOS!")/2, p.y+3, baseForeColor);
        }
    }
    else
        printfc("MÉGSE", p.x- stringlenght("MÉGSE")/2, p.y+3, baseForeColor);
    Sleep(waitms);
    if (*state == newPlayer) *state = mainMenu;
    else if (*state == editPlayer) *state = chosePlayer;
}

void PrintPlayerSubMenu(bool *displayFirst, Player **playerListHead, int *numOfPlayers, Player **currentPlayer, int selectedPlayer, Point p ) {
    if (*displayFirst){
        *displayFirst = false;
        ClearScrBellow();
        // Alcím kiírása
        printfc("JÁTÉKOSOK:", p.x-5, p.y, baseForeColor);
        // Player Listába beolvasás fájlból
        player_ReadTxtFile(playerListHead, numOfPlayers); // Játékosok kiiratásához beolvasás
    }
    // Player Lista kiiratása a képernyőre
    player_PrintPlayerList(*playerListHead, selectedPlayer, (Point) {p.x, p.y + 1} );
    // Kijelölt játékos a currentPlayerbe rakása, hogy a játék indítható legyen
    *currentPlayer = player_GetSelectedPlayer(*playerListHead, selectedPlayer);
}

void PrintRankList(bool *displayFirst, Player *playerListHead, int *numOfPlayers, Point p, int maxDisplayLvls){
    if (*displayFirst){
        *displayFirst = false;
        ClearScrBellow();
        player_ReadTxtFile(&playerListHead, numOfPlayers); // Rangsorhoz beolvasás fájlból
    }
    // Segéd változók
    Player *mover = playerListHead;
    int *spaces = (int*) malloc(*numOfPlayers * sizeof(int));
    int lenght = 0;
    int firstindent = 7; // Szint->║ Név ║
    int totalLenght = firstindent;
    int playerindex = 0;
    // Nevek hosszának megállapítása
    while (mover != NULL){
        lenght = (int) stringlenght(mover->name);
        totalLenght += lenght + 3;
        spaces[playerindex++] = lenght;
        mover = (Player *) mover->next;
    }
    // Segédváltozók visszaállítása
    p = (Point) {p.x - totalLenght/2, p.y};
    mover = playerListHead;
    playerindex = 0;
    int line = 0, maxline = 0;
    printfc(" Szint ┃ ",p.x, p.y + line++, baseForeColor);
    printfc("━━━━━━━╋ ",p.x, p.y + line++, baseForeColor);
    char text[nameLenght+5],
            helper[nameLenght+5];
    int indent = firstindent + 1;

    // Player listán végigmegyünk
    while (mover != NULL){
        line = 0;
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
            sprintf(helper, "%d\0", spaces[playerindex]);
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

    indent = firstindent + 1;
    for(int j = 0; j < *numOfPlayers - 1; j++){
        indent += spaces[j] + 3;
        for(int k = 2; k < maxline; k++) printfc("┃", p.x+indent-1, p.y+k, baseForeColor);
    }
    free(spaces);
}

void ResetMenuVars(bool *displayFirst, int *option, int *selectedPlayer) {
    *displayFirst = true;
    *option = 0;
    *selectedPlayer = 0;
}

void WarningWindow(const char* Message, Point p, int option, bool *displayFirst, EconioColor baseColor, EconioColor accentForeColor, EconioColor accentBgColor){
    int windowWidth = 26;
    if (*displayFirst) {
        *displayFirst = false;
        int i = 0;
        ClearScrBellow();
        printfc("╔════════════════════════╗", p.x-windowWidth/2,p.y+i++,baseColor);
        printfc("║                        ║", p.x-windowWidth/2,p.y+i++,baseColor);
        printfc("║", p.x-windowWidth/2,p.y+i,baseColor);
        printfc(Message, p.x- stringlenght(Message)/2, p.y+i,baseColor);
        printfc("║", p.x+windowWidth/2-1,p.y+i++,baseColor);
        printfc("║                        ║", p.x-windowWidth/2,p.y+i++,baseColor);
        printfc("║     NEM       IGEN     ║", p.x-windowWidth/2,p.y+i++,baseColor);
        printfc("╚════════════════════════╝", p.x-windowWidth/2,p.y+i--,baseColor);
    } // Kilépő ablak kiírása
    if (option == 0) {
        printfbc("NEM", p.x-windowWidth/2+6,p.y+4, accentForeColor, accentBgColor);
        printfbc("IGEN", p.x-windowWidth/2+16,p.y+4, baseColor, COL_RESET);
    }
    else {
        printfbc("NEM", p.x-windowWidth/2+6,p.y+4, baseColor, COL_RESET);
        printfbc("IGEN", p.x-windowWidth/2+16,p.y+4, accentForeColor, accentBgColor);
    }
}

void PrintEditPlayerSubMenu(){

}
void PrintNavControls(State state,  Point p, Size maxSize){
    int i = 0;
//    ClearScreenSection(0,p.y + maxDisplayLines,maxSize.width,maxSize.height, COL_RESET);
    switch (state) {
        case mainMenu:
            printfc("[↑] : Feljebb mozgás", p.x-4, p.y + maxDisplayLines + i++, COL_LIGHTBLUE);
            printfc("[↓] : Lejjebb mozgás", p.x-4, p.y + maxDisplayLines + i++, COL_LIGHTBLUE);
            printfc("[↵] : Kiválasztás", p.x-4, p.y + maxDisplayLines + i++, COL_LIGHTBLUE);
            printfc("[Esc] : Kilépés", p.x-6, p.y + maxDisplayLines + i++, COL_LIGHTBLUE);
            break;
        case newPlayer:
            printfc("[↵] : Mentés / Kilépés", p.x-4, p.y + maxDisplayLines + i++, COL_LIGHTBLUE);
            break;
        case chosePlayer:
            printfc("[↑] : Feljebb mozgás", p.x-4, p.y + maxDisplayLines + i++, COL_LIGHTBLUE);
            printfc("[↓] : Lejjebb mozgás", p.x-4, p.y + maxDisplayLines + i++, COL_LIGHTBLUE);
            printfc("[↵] : Kiválasztás", p.x-4, p.y + maxDisplayLines + i++, COL_LIGHTBLUE);
            printfc("[Esc] : Kilépés", p.x-6, p.y + maxDisplayLines + i++, COL_LIGHTBLUE);
            break;
        case rankList:
            break;
        case exitApp:
        case deletePlayer:
            break;
        case editPlayer:
            break;

    }
}
