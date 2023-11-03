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

#ifdef _WIN32
    #include <windows.h>
#endif

Point const up =  {0, -1};
Point const down = {0, 1};
Point const left = {-1, 0};
Point const right = {1, 0};
Point const corner = {25, 9};

bool Init(Player *player, char **levelList, int numOfLevels){
    // Új játék létrehozása
    ClearScrBellow();
    return StartGame(player, levelList[player->numOfCompletedLevels]);
}
bool StartGame(Player *player, char levelName[]){
    // Fő változók deklarálása
    char **map = NULL; // Pálya
    Size mapSize; // Pálya mérete
    // Játék inicializálása
    Point playerPosition;
    Point *boxPositions = NULL;
    Point *targetPositions = NULL;
    int boxCount = 0;
    ReadXSBFile(levelName, &map, &mapSize, &playerPosition, &boxPositions, &targetPositions, &boxCount); // map-be beolvassa a kapott levelName-t fájlból
    econio_rawmode(); // Billentyű módba kapcsolás
    PrintStyledMap(map,mapSize); // Pálya kiirítása

    // Játék változói
    bool runGame = true;
    bool exitMenu = false;
    int option = 0;
    bool displayFirst = true;
    bool playerCanMove = false;
    Point direction;
    int key = -1;
    // Lépéseket tároló lista
    Point *Steps = &playerPosition;
    int numOfSteps = 0;
    Point lastPoint;
    // Steps->next = NULL;
    // Játék Ciklusa
    while (runGame && !CheckWin(map, mapSize)){
        //
        key = econio_getch();
        // Evaluate input
        switch (key){
            case KEY_ESCAPE:
            case KEY_BACKSPACE:
                // Kilépés
                exitMenu = !exitMenu;
                break;
            case KEY_ENTER:
                if (exitMenu){
                    if (option == 1){ runGame = false;}
                    else {
                        exitMenu = false;
                        econio_clrscr();
                        PrintStyledMap(map,mapSize);
                    }
                }
                break;
            case KEY_UP:
                if (Move(&map, mapSize, &playerPosition, &boxPositions, targetPositions, boxCount, up)){
                    playerCanMove = true;
                    direction = up;
                }
                break;
            case KEY_DOWN:
                if (Move(&map, mapSize, &playerPosition, &boxPositions, targetPositions, boxCount, down)){
                    playerCanMove = true;
                    direction = down;
                }
                break;
            case KEY_LEFT:
                if (exitMenu) option = 0;
                else{
                    if (Move(&map, mapSize, &playerPosition, &boxPositions, targetPositions, boxCount, left)){
                        playerCanMove = true;
                        direction = left;
                    }
                }
                break;
            case KEY_RIGHT:
                if (exitMenu) option = 1;
                else {
                    if (Move(&map, mapSize, &playerPosition, &boxPositions, targetPositions, boxCount, right)){
                        playerCanMove = true;
                        direction = right;
                    }
                }
                break;
            default:
                break;
        }
        if (exitMenu){
            int _x = 25, _y = 8;
            if (displayFirst) {
                displayFirst = false;
                ClearScrBellow();
                printfc("╔════════════════════════╗", _x, _y+1, COL_RED);
                printfc("║   KILÉPSZ A JÁTÉKBÓL?  ║", _x, _y+2, COL_RED);
                printfc("║                        ║", _x, _y+3, COL_RED);
                printfc("║    NEM        IGEN     ║", _x, _y+4, COL_RED);
                printfc("╚════════════════════════╝", _x, _y+5, COL_RED);
            }
            if (option == 0) {
                printfbc("NEM", _x+5, _y+4, COL_WHITE, COL_LIGHTRED);
                printfbc("IGEN", _x+16, _y+4, COL_RED, COL_RESET);
            }
            else {
                printfbc("NEM", _x+5, _y+4, COL_RED, COL_RESET);
                printfbc("IGEN", _x+16, _y+4, COL_WHITE, COL_LIGHTRED);
            }
        }
        else{
            // Move Player
            if (playerCanMove){
                playerCanMove = false;
                //  Mező elhagyása
                char *lastCell = &map[playerPosition.y][playerPosition.x]; // Az elhagyandó mező pointere
                if (*lastCell == PLAYER) *lastCell = EMPTY;
                else /*if (*lastCell == PLAYERONTARGET)*/ *lastCell = TARGET;
                PrintPosition(map, playerPosition);
                // Új mezőre lépés
                Point destinationPoint = AddPoints(playerPosition, direction);
                char *destinationCell = &map[destinationPoint.y][destinationPoint.x]; // A lépendő mezőre mutató pointer
                if (*destinationCell == TARGET) *destinationCell = PLAYERONTARGET;
                else if (*destinationCell == EMPTY) *destinationCell = PLAYER;
                playerPosition = destinationPoint;
                PrintPosition(map, destinationPoint);
                numOfSteps++;
            }
            // Draw input to console
        }

    }
    // Játék során használt memóriaterületek felszababadítása
    FreeAllocatedMemoryFromMap(&map); // Mátrix felszabadítása
    FreeDynamicArray(&boxPositions); // Doboz tömb felszabadítása
    FreeDynamicArray(&targetPositions); // Célmező tömb felszabadítása

    if (runGame) { // A játékos teljesítete a szintet, mert a CheckWin feltétel léptette kia ciklusból
        realloc(player->levelMoves, (player->numOfCompletedLevels+1) * sizeof(int));
        player->levelMoves[player->numOfCompletedLevels] = numOfSteps;
        player->numOfCompletedLevels++;

        return true; // Következő szint
    }
    else{
        return false; // Ki akar lépni a játékos
    }

}

bool CheckWin(char **map, Size mapSize){
    for (int y = 0; y < mapSize.height; y++){
        for(int x = 0; x < mapSize.width; x++){
            if (map[y][x] == BOX) return false; // Ha talál akár egy dobozt is, ami nincs célmezőn, akkor még nem nyert a játékos
        }
    }
    return true; // Ha egy sima dobozt sem talált, akkor mind a helyén van --> Nyert
}
bool Move(char ***map, Size mapSize, Point *currentPosition, Point **boxPositions, Point *targetPositions, int boxCount, Point direction){
    char *originCell =  &((*map)[currentPosition->y][currentPosition->x]);
    Point destinationPoint = AddPoints(*currentPosition, direction);
    char *destinationCell = &((*map)[destinationPoint.y][destinationPoint.x]); // A lépendő mezőre mutató pointer. Így nem kell majd hosszú sort írni mindig
    Point boxDestinationPoint = AddPoints(destinationPoint, direction);
    char *boxDestinationCell =  &((*map)[boxDestinationPoint.y][boxDestinationPoint.x]);

    switch (*destinationCell) {
        case EMPTY: // Szabad lépés
        case TARGET: // Szabad Lépés

            return true; // Sikeres lépés
            break;
        case WALL: // Nem lehetséges lépés
            return false;
            break;
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
                return true;
            }
            else return false;
            break;
        default:
            return false;
            break;
    }
}
// Konzolra Írás
void PrintSimpleMap(char **map, Size mapSize){
    for(int y = 0; y < mapSize.height; y++){
        for(int x = 0; x < mapSize.width; x++){
            printf("%d", map[y][x]);
        }
        printf("\n");
    }
}
void PrintStyledMap(char **map, Size mapSize){
    for(int y = 0; y < mapSize.height; y++){
        for(int x = 0; x < mapSize.width; x++){
            PrintPosition(map, (Point) {x,y});
        }
    }
}
void PrintPosition(char **map, Point pos){
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

// INIT: Játék előkészítéséhez szükséges függvények
void ReadXSBFile(char filename[], char ***map, Size *mapSize, Point *playerPosition, Point **boxPositions, Point **targetPositions, int *boxCount){
    *mapSize = (Size) { 0, 0}; // pálya méretének inicializálása
    *boxCount = 0; // dobozok darabszáma == Célmezők darabszáma
    int k = 0; // map index cikluscáltozója
    int boxindex = 0; // boxPositions tömb indexelésére szolgál
    int targetindex = 0; // célmezők tömb indexelésére szolgál
    char *characterPtr; // Karakterkeresésre szolgál
    char directory[maxFileNameLenght] = "./levels/"; // xsb fájlokat tartalmazó mappa
    strcat(directory, filename); // Mappa és fájlnév egyesítése
    FILE* fp = fopen(directory, "r"); // Fájl megnyitása
    if (fp == NULL) { // Error kezelés
        printf("Nem sikerült megnyitni a fájlt: %s\n", filename);
        return;
    }
    // Sorok és oszlopok megszámlálása
    char line[maxReadLineLenght] = {0}; // Beolvasott sor a line stringbe megy

    while(fgets(line, maxReadLineLenght, fp)){ // Sor beolvasása
        // Ha sikeres, a sorokat egyel növeljük
        mapSize->height++;
        // oszlopok száma lehető legagyobb legyen, hogy minden sor elfréjen majd
        if (strlen(line)-1 > mapSize->width) mapSize->width = (int)strlen(line)-1;

        // Karakter pozíciók meghatározása
        for(int i = 0; line[i] != '\0'; i++){
            switch (line[i]) {
                case '@':
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
    FreeDynamicArray(targetPositions);
    AllocateDynamicArray(targetPositions, *boxCount + 1);

    // Tömbbe olvasás
    while(fgets(line, maxReadLineLenght, fp)){
        for(int i = 0; line[i] != '\0'; i++){
            switch(line[i]){
                case '$': // Box
                    (*boxPositions)[boxindex++] = (Point) {i, k};
                    break;
                case '*': // Box on target
                    (*boxPositions)[boxindex++] = (Point) {i, k};
                    (*targetPositions)[targetindex++] = (Point) {i, k};
                    break;
                case '+': // Player on target
                case '.': // Target
                    (*targetPositions)[targetindex++] = (Point) {i, k};
                    break;
            }
        }
        // A beolvasott sor karaktereinek kiértékelése a map mátrixba
        char* converted = ConvertInputLineToCellType(line);
        strcpy((*map)[k], converted);
        k++;
    }
    fclose(fp); // Fájl bezárása
    if (mapSize->width < 3 && mapSize->height < 3){
        perror("Hiba a fájl beolvasásánál!");
    } // Hibakezelés
}
char* ConvertInputLineToCellType(char line[]){
    int lenght = (int)strlen(line);
    int k = 0;
    for(int i = 0; i < lenght && line[i] != '\n' && line[i] != '\0'; i++){
        switch(line[i]){
            case ' ': line[k] = EMPTY; break;
            case '#': line[k] = WALL; break;
            case '.': line[k] = TARGET; break;
            case '@': line[k] = PLAYER; break;
            case '+': line[k] = PLAYERONTARGET; break;
            case '$': line[k] = BOX; break;
            case '*': line[k] = BOXONTARGET; break;
            default:  line[k] = null;
        }
        k++;
    }
    line[k] = '\0';
    return line;
}
void AllocateMemoryToMap(char ***map, Size *mapSize){
    char **newMap;
    newMap = (char**) malloc(mapSize->height * sizeof(char*)); //  Pointerre mutató pointer tömb memóriafoglalás
    if (newMap == NULL) {
        perror("Nem sikerult memoriateruletet foglalni newMap matrixnak");
        return;
    } // Hibakezelés
    newMap[0] = (char*) malloc((mapSize->height * mapSize->width) * sizeof(char)); // Mátrix memóriafoglalása
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
void FreeAllocatedMemoryFromMap(char ***map){
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