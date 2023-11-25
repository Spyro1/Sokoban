#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../libraries/debugmalloc.h"
#include "../libraries/econio.h"
#include "../headers/datatypes.h"
#include "../headers/game.h"
#include "../headers/move.h"
#include "../headers/lib.h"
#include "../headers/statistics.h"

Point const up =  {0, -1};
Point const down = {0, 1};
Point const left = {-1, 0};
Point const right = {1, 0};
Point const corner = {20, 9};

// Játék globális változói
bool exitMenu = false;      // Leálljon e a játék
bool runGame = true;        // Amíg igaz, addig fut a játék ciklusa, hamisnál leáll a játék, és visszatér a menübe
int option = 0;             // Aktuálisan kiválasztott opció
bool displayFirst = true;   // Igaz, ha egy menüponba először megyünk bele, Hamis, ha már a többedjére
int key;                    // Lenyomott billentyű kódja


// Konzolra Írás
static void game_PrintPosition(CellType **map, Point pos){
    econio_gotoxy(corner.x + pos.x, corner.y + pos.y);
    econio_textbackground(COL_RESET);
    econio_textcolor(COL_RESET);
    switch(map[pos.y][pos.x]){
        case WALL:
            econio_textcolor(clrWall);
            printf(chrWall);
            break;
        case TARGET:
            econio_textcolor(clrTarget);
            printf(chrTarget);
            break;
        case PLAYER:
            econio_textcolor(clrPlayer);
            printf(chrPlayer);
            break;
        case PLAYERONTARGET:
            econio_textcolor(clrPlayerOnTarget);
            printf(chrPlayer);
            break;
        case BOX:
            econio_textcolor(clrBox);
            printf(chrBox);
            break;
        case BOXONTARGET:
            econio_textcolor(clrBoxOnTarget);
            printf(chrBox);
            break;
        default:
            printf(" ");
            break;
    }
}
static void game_PrintStyledMap(CellType **map, Size mapSize){
    for(int y = 0; y < mapSize.height; y++){
        for(int x = 0; x < mapSize.width; x++){
            game_PrintPosition(map, (Point) {x, y});
        }
    }
}
static void game_PrintStatsAndNav(Size mapSize, int numOfSteps, int level){
    char printer[maxLineLenght];
    Point p = {corner.x + mapSize.width + 3, corner.y};
    int i = 2;
    if (level == 0) {
        printfc("Szint: Tutorial", p.x, p.y, baseForeColor);
        printfc("Mozgáshoz használd", p.x, p.y+i++, baseForeColor);
        printfc("a kurzor billentyűket.", p.x, p.y+i++, baseForeColor);
        printfc("A célod, hogy a sárga", p.x, p.y+i++, baseForeColor);
        printfc("dobozokat a helyükre told. ", p.x, p.y+i++, baseForeColor);

        sprintf(printer, "Fal: %s", chrWall);
        printfc(printer, p.x, p.y+i++, clrWall);
        sprintf(printer, "Játékos: %s", chrPlayer);
        printfc(printer, p.x, p.y+i++, clrPlayer);
        sprintf(printer, "Doboz: %s", chrBox);
        printfc(printer, p.x, p.y+i++, clrBox);
        sprintf(printer, "Doboz a helyén: %s", chrBox);
        printfc(printer, p.x, p.y+i++, clrBoxOnTarget);
        sprintf(printer, "Célmező: %s", chrTarget);
        printfc(printer, p.x, p.y+i++, clrTarget);
        printfc("Jó játékot!", p.x, p.y+i, baseForeColor);
    }
    else{
        i = 0;
        sprintf(printer, "Szint: %d", level);
        printfc(printer, p.x, p.y + i++, baseForeColor);
        sprintf(printer, "Lépés: %d", numOfSteps);
        printfc(printer, p.x, p.y+i, baseForeColor);
//        i++;
//        printfc("[V] : Visszalépés", p.x, p.y + i++, baseForeColor);
//        printfc("[R] : Szint újrakezése", p.x, p.y + i++, baseForeColor);
//        printfc("[Esc] : Kilépés/Mentés", p.x, p.y + i, baseForeColor);
    }
}

// = Lépés függvények  =
static bool game_CheckWin(CellType **map, Size mapSize){
    for (int y = 0; y < mapSize.height; y++){
        for(int x = 0; x < mapSize.width; x++){
            if (map[y][x] == BOX) return false; // Ha talál akár egy dobozt is, ami nincs célmezőn, akkor még nem nyert a játékos
        }
    }
    return true; // Ha egy sima dobozt sem talált, akkor mind a helyén van --> Nyert
}
static bool game_MovePlayer(CellType ***map, Point *currentPosition, Point **boxPositions, Point direction, Move **movesListHead){
    CellType *originCell =  &((*map)[currentPosition->y][currentPosition->x]);
    Point destinationPoint = addPoints(*currentPosition, direction);
    CellType *destinationCell = &((*map)[destinationPoint.y][destinationPoint.x]); // A lépendő mezőre mutató pointer. Így nem kell majd hosszú sort írni mindig
    Point boxDestinationPoint = addPoints(destinationPoint, direction);
    CellType *boxDestinationCell =  &((*map)[boxDestinationPoint.y][boxDestinationPoint.x]);

    switch (*destinationCell) {
        case EMPTY: // Szabad lépés
        case TARGET: // Szabad Lépés
            //  Mező elhagyása
            if (*originCell == PLAYER) *originCell = EMPTY;
            else *originCell = TARGET;
            game_PrintPosition(*map, *currentPosition);
            // Új mezőre lépés
            if (*destinationCell == TARGET) *destinationCell = PLAYERONTARGET;
            else if (*destinationCell == EMPTY) *destinationCell = PLAYER;
            // Lépés eltárolása
            move_AddMoveToList(move_CreateMove(*currentPosition, destinationPoint, false), movesListHead);
            *currentPosition = destinationPoint;
            game_PrintPosition(*map, destinationPoint);

            return true; // Sikeres lépés
            //break;
        case WALL: // Nem lehetséges lépés
            return false;
            //break;
        case BOX: // Doboz eltolás
        case BOXONTARGET: // Doboz eltolás
            // Megnézzük, hogy tudnánk e a dobozt tolni egyel odébb
            if (*boxDestinationCell == EMPTY || *boxDestinationCell == TARGET){
                int i = 0;
                while (!comparePoints((*boxPositions)[i], destinationPoint)){ i++; } // Aktuális helyen lévő doboz megkeresése
                // Régi mező elhagyása
                if (*destinationCell == BOXONTARGET) *destinationCell = TARGET;
                else /*if (*destinationCell == BOX)*/ *destinationCell = EMPTY;
                game_PrintPosition(*map, destinationPoint);
                // Doboz Új mezőre léptetése
                if (*boxDestinationCell == TARGET) *boxDestinationCell = BOXONTARGET;
                else *boxDestinationCell = BOX;
                (*boxPositions)[i] = boxDestinationPoint;
                game_PrintPosition(*map, boxDestinationPoint);

                // Player átmozgatása
                //  Mező elhagyása
                if (*originCell == PLAYER) *originCell = EMPTY;
                else *originCell = TARGET;
                game_PrintPosition(*map, *currentPosition);
                // Új mezőre lépés
                if (*destinationCell == TARGET) *destinationCell = PLAYERONTARGET;
                else if (*destinationCell == EMPTY) *destinationCell = PLAYER;
                // Lépés eltárolása
                move_AddMoveToList(move_CreateMove(*currentPosition, destinationPoint, true), movesListHead);
                *currentPosition = destinationPoint;
                game_PrintPosition(*map, destinationPoint);
                return true;
            }
            else return false;
            //break;
        default:
            return false;
            //break;
    }
}
static bool game_UndoMove(CellType ***map, Point *currentPosition, Point **boxPositions, Move **moveListHead){
    // Ha volt már elmozdulás
    if (*moveListHead != NULL){
        Move lastMove = move_RemoveMoveFromList(moveListHead);
        Point moveDirection = subPoints(lastMove.to, lastMove.from);
//        Point undoDirection = subPoints(lastMove.from, lastMove.to);
        Point boxPosition = addPoints(lastMove.to, moveDirection);

        CellType *FromCell = &((*map)[lastMove.from.y][lastMove.from.x]);
        CellType *ToCell =  &((*map)[lastMove.to.y][lastMove.to.x]);
        CellType *BoxCell =  &((*map)[boxPosition.y][boxPosition.x]);

        // Visszaléptetés
        *currentPosition = lastMove.from;
        if (*FromCell == TARGET)
            *FromCell = PLAYERONTARGET;
        else
            *FromCell = PLAYER;
        if (lastMove.boxPushed){
            if (*BoxCell == BOX){
                *BoxCell = EMPTY;
                int i = 0;
                while (!comparePoints((*boxPositions)[i], boxPosition)){ i++; } // Aktuális helyen lévő doboz megkeresése
                (*boxPositions)[i] = lastMove.to;
                game_PrintPosition(*map, boxPosition);
                if (*ToCell == PLAYERONTARGET) *ToCell = BOXONTARGET;
                else *ToCell = BOX;
            }
            else if (*BoxCell == BOXONTARGET) {
                *BoxCell = TARGET;
                int i = 0;
                while (!comparePoints((*boxPositions)[i],
                                      boxPosition)) { i++; } // Aktuális helyen lévő doboz megkeresése
                (*boxPositions)[i] = lastMove.to;
                game_PrintPosition(*map, boxPosition);
                if (*ToCell == PLAYERONTARGET) *ToCell = BOXONTARGET;
                else *ToCell = BOX;
            }
        }
        else{
            if (*ToCell == PLAYERONTARGET) *ToCell = TARGET;
            else *ToCell = EMPTY;
        }
        game_PrintPosition(*map, lastMove.from);
        game_PrintPosition(*map, lastMove.to);
        return true;
    }
    else
        return false;
}

// = INIT: Játék előkészítéséhez szükséges függvények =
static CellType game_ConvertInputCharToCellType(char character){
    switch(character){
        case ' ': return EMPTY; //break;
        case '#': return WALL; //break;
        case '.': return TARGET; //;
        case '@': return PLAYER; //break;
        case '+': return PLAYERONTARGET; //break;
        case '$': return BOX; //break;
        case '*': return BOXONTARGET; //break;
        default: return null;
    }
}
static void game_AllocateMemoryToMap(CellType ***map, Size *mapSize){
    CellType **newMap;
    newMap = (CellType**) malloc(mapSize->height * sizeof(CellType*)+1); //  Pointerre mutató pointer tömb memóriafoglalás
    if (newMap == NULL) {
        lib_printError("Nem sikerült memóriaterületet foglalni newMap mátrixnak.");
        return;
    } // Hibakezelés
    newMap[0] = (CellType*) malloc((mapSize->height * mapSize->width+1) * sizeof(CellType)); // Mátrix memóriafoglalása
    if (newMap[0] == NULL) {
        lib_printError("Nem sikerült memóriaterületet fogallni a newMap[0] tömbnek.");
        return;
    } // Hibakezelés
    // Pointerek értékadása
    for (int y = 1; y < mapSize->height; y++){
        newMap[y] = newMap[0] + y * mapSize->width;
    }
    *map = newMap; // Címátadás
}
static void game_AllocateDynamicArray(Point **newArray, int lenght){
    Point *helperArray;
    helperArray = (Point*) malloc(lenght * sizeof(Point)); // Dobozok tömbjének memóriafoglalása
    if(helperArray == NULL) {
        lib_printError("Nem sikerült memóriat foglalni a newArray tömbnek.");
        return;
    } // Hibakezelés
    *newArray = helperArray; // Címátadás
}
static void game_FreeAllocatedMemoryFromMap(CellType ***map){
    if (*map != NULL) {
        if ((*map)[0] != NULL)
            free((*map)[0]);
        free(*map);
    }
}
static void game_FreeDynamicArray(Point **dynamicArray){
    if (*dynamicArray != NULL){
        free(*dynamicArray);
    }
}
static void game_ReadXSBFile(char filename[], CellType ***map, Size *mapSize, Point *playerPosition, Point **boxPositions, int *boxCount){
    *mapSize = (Size) { 0, 0}; // pálya méretének inicializálása
    *boxCount = 0; // dobozok darabszáma == Célmezők darabszáma
    int k = 0; // map index cikluscáltozója
    int boxindex = 0; // boxPositions tömb indexelésére szolgál
    char directory[maxFileNameLenght] = "./levels/"; // xsb fájlokat tartalmazó mappa
    strcat(directory, filename); // Mappa és fájlnév egyesítése
    FILE* fp = fopen(directory, "r"); // Fájl megnyitása
    if (fp == NULL) { // Error kezelés
        sprintf(directory, "Nem sikerült megnyitni a fájlt pálya fájlját: %s", filename);
        lib_printError(directory);
        return;
    }
    // Sorok és oszlopok megszámlálása
    char line[maxLineLenght] = {0}; // Beolvasott sor a line stringbe megy

    while(fgets(line, maxLineLenght, fp)){ // Sor beolvasása
        // Ha sikeres, a sorokat egyel növeljük
        mapSize->height++;
        // oszlopok száma lehető legagyobb legyen, hogy minden sor elfréjen majd
        if (strlen(line)-1 > mapSize->width) mapSize->width = (int)strlen(line)-1;

        // Karakter pozíciók meghatározása
        for(int i = 0; line[i] != '\0'; i++){
            switch (line[i]) {
                case '@':
                case '+':
                    *playerPosition = (Point) {i, mapSize->height-1};
                    break;
                case '$':
                case '*':
                    (*boxCount)++; // Dobozok számának növelése
                    break;
                default:
                    break;
            }
        }
    }
    rewind(fp);

    // Pálya Mátrix felszabadítása és foglalása
    game_FreeAllocatedMemoryFromMap(map);
    game_AllocateMemoryToMap(map, mapSize);
    // Dobozok és célmező tömb felszabadítása és foglalása
    game_FreeDynamicArray(boxPositions);
    game_AllocateDynamicArray(boxPositions, *boxCount + 1);

    // Tömbbe olvasás
    while(fgets(line, maxLineLenght, fp)){
        for(int i = 0; line[i] != '\0'; i++){
            // A beolvasott sor karaktereinek kiértékelése a map mátrixba
            (*map)[k][i] = game_ConvertInputCharToCellType(line[i]);
            switch(line[i]){
                case '$': // Box
                case '*': // Box on target
                    (*boxPositions)[boxindex++] = (Point) {i, k};
                    break;
            }
        }
        k++;
    }
    fclose(fp); // Fájl bezárása
    if (mapSize->width < 3 && mapSize->height < 3){
        lib_printError("Túl kicsi a pálya a játékhoz!");
    } // Hibakezelés
}

// = Játék függvényei =
static bool game_KeyPress(CellType **map, Size mapSize, int *numOfMoves, Player *player, Point *playerPosition, Point *boxPositions, Move **movesListHead){
    switch (key){
        case KEY_ESCAPE:
        case KEY_BACKSPACE:
            exitMenu = !exitMenu; // Kilépés
            displayFirst = true;
            if (!exitMenu)
            {
                displayFirst = true;
                lib_ClearScrBellow();
                game_PrintStyledMap(map, mapSize); // Pálya kiirítása
                game_PrintStatsAndNav(mapSize, 0, player->numOfCompletedLevels);
            }
            break;
        case KEY_ENTER:
            if (exitMenu){
                if (option == 1){
                    runGame = false;
                    exitMenu = false;
                }
                else {
                    exitMenu = false;
                    displayFirst = true;
                    lib_ClearScrBellow();
                    game_PrintStyledMap(map, mapSize);
                    game_PrintStatsAndNav(mapSize, *numOfMoves, player->numOfCompletedLevels);
                }
            }
            break;
        case KEY_UP:
            if (game_MovePlayer(&map, playerPosition, &boxPositions, up, movesListHead)){
                (*numOfMoves)++;
                game_PrintStatsAndNav(mapSize, *numOfMoves, player->numOfCompletedLevels); // PRINT STATS
            }
            break;
        case KEY_DOWN:
            if (game_MovePlayer(&map, playerPosition, &boxPositions, down, movesListHead)){
                (*numOfMoves)++;
                game_PrintStatsAndNav(mapSize, *numOfMoves, player->numOfCompletedLevels); // PRINT STATS
            }
            break;
        case KEY_LEFT:
            if (exitMenu) option = 0;
            else{
                if (game_MovePlayer(&map, playerPosition, &boxPositions, left, movesListHead)){
                    (*numOfMoves)++;
                    game_PrintStatsAndNav(mapSize, *numOfMoves, player->numOfCompletedLevels); // PRINT STATS
                }
            }
            break;
        case KEY_RIGHT:
            if (exitMenu) option = 1;
            else {
                if (game_MovePlayer(&map, playerPosition, &boxPositions, right, movesListHead)){
                    (*numOfMoves)++;
                    game_PrintStatsAndNav(mapSize, *numOfMoves, player->numOfCompletedLevels); // PRINT STATS
                }
            }
            break;
        case 'V':
        case 'v':
            // Visszalépés
            if (game_UndoMove(&map, playerPosition, &boxPositions, movesListHead)){
                (*numOfMoves)++;
                game_PrintStatsAndNav(mapSize, *numOfMoves, player->numOfCompletedLevels); // PRINT STATS
            }
            break;
        case 'R':
        case 'r':
            // Játék során használt memóriaterületek felszababadítása
            move_FreeMoveList(movesListHead); // Elmozdulásokat regisztráló láncoltl ista felszabadítása
            game_FreeAllocatedMemoryFromMap(&map); // Mátrix felszabadítása
            game_FreeDynamicArray(&boxPositions); // Doboz tömb felszabadítása
            // Pálya resetelése
            return true;
            //break;
        default:
            break;
    }
    return false;
}
static bool game_StartGame(Player *player, char levelName[]){
    // Fő változók deklarálása
    CellType **map = NULL; // Pálya
    Size mapSize; // Pálya mérete

    // Konstansok a kiiratáshoz
    const int center = 36; // Képernyő közepe a cím szerint
    Point p;               // A kiiratás középpontja a cím alatt

    // Játék inicializálása
    Point playerPosition;
    Point *boxPositions = NULL;
    int boxCount = 0;
    exitMenu = false;
    runGame = true;
    option = 0;
    displayFirst = true;
    game_ReadXSBFile(levelName, &map, &mapSize, &playerPosition, &boxPositions, &boxCount); // map-be beolvassa a kapott levelName-t fájlból
    econio_rawmode(); // Billentyű módba kapcsolás
    game_PrintStyledMap(map, mapSize); // Pálya kiirítása
    game_PrintStatsAndNav(mapSize, 0, player->numOfCompletedLevels);

    // Lépéseket tároló lista
    Move *playerMovesListHead = NULL;
    int numOfMoves = 0;
    // Játék Ciklusa
    while (runGame && !game_CheckWin(map, mapSize)){
        // Billentyűlenyomás érzékelése
        key = econio_getch();
        p = (Point) {center, 9};
        // Lenyomott billentyű kiértékelése
        if (game_KeyPress(map, mapSize, &numOfMoves, player, &playerPosition, boxPositions, &playerMovesListHead)){
            return true;
        }

        // Kilépés Kiírása
        if (exitMenu){
            lib_WarningWindow("KILÉPSZ A JÁTÉKBÓL?", p, &displayFirst, option, COL_RED, COL_WHITE, COL_LIGHTRED);
        }
    }
    // Játék során használt memóriaterületek felszababadítása
    move_FreeMoveList(&playerMovesListHead);     // Elmozdulásokat regisztráló láncoltl ista felszabadítása
    game_FreeAllocatedMemoryFromMap(&map);           // Mátrix felszabadítása
    game_FreeDynamicArray(&boxPositions);        // Doboz tömb felszabadítása

    if (runGame) { // A játékos teljesítete a szintet, mert a game_CheckWin feltétel léptette kia ciklusból
        if (player->numOfCompletedLevels > 0)
            stats_AddLevelStatistics(numOfMoves, (Statistics **) &(player->levelStats));
        (player->numOfCompletedLevels)++;
        return true; // Következő szint
    }
    else{
        return false; // Ki akar lépni a játékos
    }

}
// = Kívülről meghívandó függvény =
bool game_Init(Player *player, char **levelList){
    // Új játék létrehozása
    lib_ClearScrBellow();
    return game_StartGame(player, levelList[player->numOfCompletedLevels]);
}