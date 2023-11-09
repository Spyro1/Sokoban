#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libraries/debugmalloc.h"
#include "../libraries/econio.h"
#include "../headers/datatypes.h"
#include "../headers/game.h"
#include "../headers/move.h"
#include "../headers/statistics.h"

Point const up =  {0, -1};
Point const down = {0, 1};
Point const left = {-1, 0};
Point const right = {1, 0};
Point const corner = {20, 9};

bool Init(Player *player, char **levelList){
    // Új játék létrehozása
    ClearScrBellow();
    return StartGame(player, levelList[player->numOfCompletedLevels]);
}
bool StartGame(Player *player, char levelName[]){
    // Fő változók deklarálása
    CellType **map = NULL; // Pálya
    Size mapSize; // Pálya mérete

    // Konstansok a kiiratáshoz
    const int center = 36; // Képernyő közepe a cím szerint
    const int maxDisplayLines = 10;
    Point p = {center, 9}; // A kiiratás középpontja a cím alatt

    // Játék inicializálása
    Point playerPosition;
    Point *boxPositions = NULL;
    Point *targetPositions = NULL;
    int boxCount = 0;
    ReadXSBFile(levelName, &map, &mapSize, &playerPosition, &boxPositions, &boxCount); // map-be beolvassa a kapott levelName-t fájlból
    econio_rawmode(); // Billentyű módba kapcsolás
    PrintStyledMap(map,mapSize); // Pálya kiirítása
    PrintStatsAndNav(mapSize, 0, player->numOfCompletedLevels);
    // Játék változói
    bool exitMenu = false;
    bool runGame = true;
    int option = 0;
    bool displayFirst = true;
    int key;
    // Lépéseket tároló lista
    Move *PlayerMovesListHead = NULL;
    int numOfMoves = 0;
    // Játék Ciklusa
    while (runGame && !CheckWin(map, mapSize)){
        // Billentyűlenyomás érzékelése
        key = econio_getch();
        p = (Point) {center, 9};
        // Lenyomott billentyű kiértékelése
        switch (key){
            case KEY_ESCAPE:
            case KEY_BACKSPACE:
                exitMenu = !exitMenu; // Kilépés
                displayFirst = true;
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
                        ClearScrBellow();
                        PrintStyledMap(map,mapSize);
                        PrintStatsAndNav(mapSize, numOfMoves, player->numOfCompletedLevels);
                    }
                }
                break;
            case KEY_UP:
                if (MovePlayer(&map, &playerPosition, &boxPositions, up, &PlayerMovesListHead)){
                    numOfMoves++;
                    PrintStatsAndNav(mapSize, numOfMoves, player->numOfCompletedLevels); // PRINT STATS
                }
                break;
            case KEY_DOWN:
                if (MovePlayer(&map, &playerPosition, &boxPositions, down, &PlayerMovesListHead)){
                    numOfMoves++;
                    PrintStatsAndNav(mapSize, numOfMoves, player->numOfCompletedLevels); // PRINT STATS
                }
                break;
            case KEY_LEFT:
                if (exitMenu) option = 0;
                else{
                    if (MovePlayer(&map, &playerPosition, &boxPositions, left, &PlayerMovesListHead)){
                        numOfMoves++;
                        PrintStatsAndNav(mapSize, numOfMoves, player->numOfCompletedLevels); // PRINT STATS
                    }
                }
                break;
            case KEY_RIGHT:
                if (exitMenu) option = 1;
                else {
                    if (MovePlayer(&map, &playerPosition, &boxPositions, right, &PlayerMovesListHead)){
                        numOfMoves++;
                        PrintStatsAndNav(mapSize, numOfMoves, player->numOfCompletedLevels); // PRINT STATS
                    }
                }
                break;
            case 'V':
            case 'v':
                // Visszalépés
                if (UndoMove(&map,&playerPosition,&boxPositions,&PlayerMovesListHead)){
                    numOfMoves++;
                    PrintStatsAndNav(mapSize, numOfMoves, player->numOfCompletedLevels); // PRINT STATS
                }
                break;
            case 'R':
            case 'r':
                // Játék során használt memóriaterületek felszababadítása
                FreeMoveList(&PlayerMovesListHead); // Elmozdulásokat regisztráló láncoltl ista felszabadítása
                FreeAllocatedMemoryFromMap(&map); // Mátrix felszabadítása
                FreeDynamicArray(&boxPositions); // Doboz tömb felszabadítása
                FreeDynamicArray(&targetPositions); // Célmező tömb felszabadítása
                // Pálya resetelése
                return true;
                break;
            default:
                break;
        }
        // Kiírás
        if (exitMenu){
            WarningWindow("KILÉPSZ A JÁTÉKBÓL?", p, option, &displayFirst, COL_RED, COL_WHITE, COL_LIGHTRED);
        }
    }
    // Játék során használt memóriaterületek felszababadítása
    FreeMoveList(&PlayerMovesListHead);     // Elmozdulásokat regisztráló láncoltl ista felszabadítása
    FreeAllocatedMemoryFromMap(&map);           // Mátrix felszabadítása
    FreeDynamicArray(&boxPositions);        // Doboz tömb felszabadítása
    FreeDynamicArray(&targetPositions);     // Célmező tömb felszabadítása

    if (runGame) { // A játékos teljesítete a szintet, mert a CheckWin feltétel léptette kia ciklusból
        if (player->numOfCompletedLevels > 0)
            AddLevelStatistics(numOfMoves, (Statistics **) &(player->levelStats));
        (player->numOfCompletedLevels)++;
        return true; // Következő szint
    }
    else{
        return false; // Ki akar lépni a játékos
    }

}

bool CheckWin(CellType **map, Size mapSize){
    for (int y = 0; y < mapSize.height; y++){
        for(int x = 0; x < mapSize.width; x++){
            if (map[y][x] == BOX) return false; // Ha talál akár egy dobozt is, ami nincs célmezőn, akkor még nem nyert a játékos
        }
    }
    return true; // Ha egy sima dobozt sem talált, akkor mind a helyén van --> Nyert
}
bool MovePlayer(CellType ***map, Point *currentPosition, Point **boxPositions, Point direction, Move **movesListHead){
    CellType *originCell =  &((*map)[currentPosition->y][currentPosition->x]);
    Point destinationPoint = AddPoints(*currentPosition, direction);
    CellType *destinationCell = &((*map)[destinationPoint.y][destinationPoint.x]); // A lépendő mezőre mutató pointer. Így nem kell majd hosszú sort írni mindig
    Point boxDestinationPoint = AddPoints(destinationPoint, direction);
    CellType *boxDestinationCell =  &((*map)[boxDestinationPoint.y][boxDestinationPoint.x]);

    switch (*destinationCell) {
        case EMPTY: // Szabad lépés
        case TARGET: // Szabad Lépés
            //  Mező elhagyása
            if (*originCell == PLAYER) *originCell = EMPTY;
            else *originCell = TARGET;
            PrintPosition(*map, *currentPosition);
            // Új mezőre lépés
            if (*destinationCell == TARGET) *destinationCell = PLAYERONTARGET;
            else if (*destinationCell == EMPTY) *destinationCell = PLAYER;
            // Lépés eltárolása
            AddMoveToList(CreateMove(*currentPosition, destinationPoint, false), movesListHead);
            *currentPosition = destinationPoint;
            PrintPosition(*map, destinationPoint);

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
                while (!EqualToPoint((*boxPositions)[i],destinationPoint)){ i++; } // Aktuális helyen lévő doboz megkeresése
                // Régi mező elhagyása
                if (*destinationCell == BOXONTARGET) *destinationCell = TARGET;
                else /*if (*destinationCell == BOX)*/ *destinationCell = EMPTY;
                PrintPosition(*map, destinationPoint);
                // Doboz Új mezőre léptetése
                if (*boxDestinationCell == TARGET) *boxDestinationCell = BOXONTARGET;
                else *boxDestinationCell = BOX;
                (*boxPositions)[i] = boxDestinationPoint;
                PrintPosition(*map, boxDestinationPoint);

                // Player átmozgatása
                //  Mező elhagyása
                if (*originCell == PLAYER) *originCell = EMPTY;
                else *originCell = TARGET;
                PrintPosition(*map, *currentPosition);
                // Új mezőre lépés
                if (*destinationCell == TARGET) *destinationCell = PLAYERONTARGET;
                else if (*destinationCell == EMPTY) *destinationCell = PLAYER;
                // Lépés eltárolása
                AddMoveToList(CreateMove(*currentPosition, destinationPoint, true), movesListHead);
                *currentPosition = destinationPoint;
                PrintPosition(*map, destinationPoint);
                return true;
            }
            else return false;
            //break;
        default:
            return false;
            //break;
    }
}
bool UndoMove(CellType ***map, Point *currentPosition, Point **boxPositions, Move **moveListHead){
    // Ha volt már elmozdulás
    if (*moveListHead != NULL){
        Move lastMove = RemoveMoveFromList(moveListHead);
        Point moveDirection = SubPoints(lastMove.to, lastMove.from);
//        Point undoDirection = SubPoints(lastMove.from, lastMove.to);
        Point boxPosition = AddPoints(lastMove.to, moveDirection);

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
                while (!EqualToPoint((*boxPositions)[i],boxPosition)){ i++; } // Aktuális helyen lévő doboz megkeresése
                (*boxPositions)[i] = lastMove.to;
                PrintPosition(*map,boxPosition);
                if (*ToCell == PLAYERONTARGET) *ToCell = BOXONTARGET;
                else *ToCell = BOX;
            }
            else if (*BoxCell == BOXONTARGET) {
                *BoxCell = TARGET;
                int i = 0;
                while (!EqualToPoint((*boxPositions)[i],
                                     boxPosition)) { i++; } // Aktuális helyen lévő doboz megkeresése
                (*boxPositions)[i] = lastMove.to;
                PrintPosition(*map, boxPosition);
                if (*ToCell == PLAYERONTARGET) *ToCell = BOXONTARGET;
                else *ToCell = BOX;
            }
        }
        else{
            if (*ToCell == PLAYERONTARGET) *ToCell = TARGET;
            else *ToCell = EMPTY;
        }
//        if (*ToCell == PLAYERONTARGET) *ToCell = (*BoxCell == TARGET) ? BOXONTARGET : BOX;
//        else *ToCell = (*BoxCell == TARGET) ? TARGET : EMPTY;
        PrintPosition(*map,lastMove.from);
        PrintPosition(*map,lastMove.to);
        return true;
    }
    else
        return false;
}
// Konzolra Írás
void PrintSimpleMap(CellType **map, Size mapSize){
    for(int y = 0; y < mapSize.height; y++){
        for(int x = 0; x < mapSize.width; x++){
            printf("%c", map[y][x]);
        }
        printf("\n");
    }
}
void PrintStyledMap(CellType **map, Size mapSize){
    for(int y = 0; y < mapSize.height; y++){
        for(int x = 0; x < mapSize.width; x++){
            PrintPosition(map, (Point) {x,y});
        }
    }
}
void PrintPosition(CellType **map, Point pos){
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
void PrintStatsAndNav(Size mapSize, int numOfSteps, int level){
    char printer[maxLineLenght];
    Point p = {corner.x + mapSize.width + 3, corner.y};
    int i = 2;
    if (level == 0) {
        printfc("Szint: Tutorial", p.x, p.y, baseForeColor);
        printfc("Mozgáshoz használd", p.x, p.y+i++, baseForeColor);
        printfc("a kurzor billentyűket.", p.x, p.y+i++, baseForeColor);
        printfc("A célod, hogy a barna", p.x, p.y+i++, baseForeColor);
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
        printfc("Jó játékot!", p.x, p.y+i++, baseForeColor);
    }
    else{
        i = 0;
        sprintf(printer, "Szint: %d", level);
        printfc(printer, p.x, p.y + i++, baseForeColor);
        sprintf(printer, "Lépés: %d", numOfSteps);
        printfc(printer, p.x, p.y+i++, baseForeColor);
        i++;
        printfc("[V] : Visszalépés", p.x, p.y + i++, baseForeColor);
        printfc("[R] : Szint reset", p.x, p.y + i++, baseForeColor);
        printfc("[Esc] : Kilépés", p.x, p.y + i++, baseForeColor);
    }
}


// INIT: Játék előkészítéséhez szükséges függvények
void ReadXSBFile(char filename[], CellType ***map, Size *mapSize, Point *playerPosition, Point **boxPositions, int *boxCount){
    *mapSize = (Size) { 0, 0}; // pálya méretének inicializálása
    *boxCount = 0; // dobozok darabszáma == Célmezők darabszáma
    int k = 0; // map index cikluscáltozója
    int boxindex = 0; // boxPositions tömb indexelésére szolgál
    char directory[maxFileNameLenght] = "./levels/"; // xsb fájlokat tartalmazó mappa
    strcat(directory, filename); // Mappa és fájlnév egyesítése
    FILE* fp = fopen(directory, "r"); // Fájl megnyitása
    if (fp == NULL) { // Error kezelés
        printf("Nem sikerült megnyitni a fájlt: %s\n", filename);
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
                    (*boxCount)++; // Doboz és célmezők számának növelése
                    break;
                default:
                    break;
            }
        }
    }
    rewind(fp);

    // Pálya Mátrix felszabadítása és foglalása
    FreeAllocatedMemoryFromMap(map);
    AllocateMemoryToMap(map, mapSize);
    // Dobozok és célmező tömb felszabadítása és foglalása
    FreeDynamicArray(boxPositions);
    AllocateDynamicArray(boxPositions, *boxCount + 1);
//    FreeDynamicArray(targetPositions);
//    AllocateDynamicArray(targetPositions, *boxCount + 1);

    // Tömbbe olvasás
    while(fgets(line, maxLineLenght, fp)){
        for(int i = 0; line[i] != '\0'; i++){
            // A beolvasott sor karaktereinek kiértékelése a map mátrixba
            (*map)[k][i] = ConvertInputCharToCellType(line[i]);
            switch(line[i]){
                case '$': // Box
                case '*': // Box on target
                    (*boxPositions)[boxindex++] = (Point) {i, k};
                    break;
//                    (*boxPositions)[boxindex++] = (Point) {i, k};
//                    (*targetPositions)[targetindex++] = (Point) {i, k};
//                    break;
//                case '+': // Player on target
//                case '.': // Target
//                    (*targetPositions)[targetindex++] = (Point) {i, k};
//                    break;
            }
        }

        k++;
    }
    fclose(fp); // Fájl bezárása
    if (mapSize->width < 3 && mapSize->height < 3){
        perror("Hiba a fájl beolvasásánál!");
    } // Hibakezelés
}
CellType ConvertInputCharToCellType(char character){
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
void AllocateMemoryToMap(CellType ***map, Size *mapSize){
    CellType **newMap;
    newMap = (CellType**) malloc(mapSize->height * sizeof(CellType*)+1); //  Pointerre mutató pointer tömb memóriafoglalás
    if (newMap == NULL) {
        perror("Nem sikerult memoriateruletet foglalni newMap matrixnak");
        return;
    } // Hibakezelés
    newMap[0] = (CellType*) malloc((mapSize->height * mapSize->width+1) * sizeof(CellType)); // Mátrix memóriafoglalása
    if (newMap[0] == NULL) {
        perror("Nem sikerult memoriateruletet fogallni a newMap[0] tombnek");
        return;
    } // Hibakezelés
    // Pointerek értékadása
    for (int y = 1; y < mapSize->height; y++){
        newMap[y] = newMap[0] + y * mapSize->width;
    }
    *map = newMap; // Címátadás
}
void AllocateDynamicArray(Point **newArray, int lenght){
    Point *helperArray;
    helperArray = (Point*) malloc(lenght * sizeof(Point)); // Dobozok tömbjének memóriafoglalása
    if(helperArray == NULL) {
        perror("Nem sikerult memoriat foglalni a newArray tömbnek.");
        return;
    } // Hibakezelés
    *newArray = helperArray; // Címátadás
}
void FreeAllocatedMemoryFromMap(CellType ***map){
    if (*map != NULL) {
        if ((*map)[0] != NULL)
            free((*map)[0]); // Itt  valamiért felszabadításkor a 370. sorról  hiba van túlindexelés
        free(*map);
    }
}
void FreeDynamicArray(Point **dynamicArray){
    if (*dynamicArray != NULL){
        free(*dynamicArray);
    }
}