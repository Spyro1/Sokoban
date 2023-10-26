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
Point const corner = {5, 5};

void Init(Player *player, char *levelList[], int numOfLevels, int selectedLevel){
    // Fő változók deklarálása
    char **map = NULL; // Pálya
    Size mapSize; // Pálya mérete
    // Új játék létrehozása
    NewGame(player, &map, mapSize,levelList[selectedLevel]);
    // Free map
    //FreeAllocatedMemoryFromMap(&map);

}
void NewGame(Player *player, char ***map, Size mapSize, char levelName[]){
    Point playerPosition;
    Point *boxPositions = NULL;
    Point *targetPositions = NULL;
    int boxCount = 0;
    ReadXSBFile(levelName, map, &mapSize, &playerPosition, &boxPositions, &targetPositions, &boxCount); // map-be beolvassa a kapott levelName-t fájlból

    econio_rawmode(); // Billentyű módba kapcsolás
    PrintStyledMap(*map,mapSize); // Pálya kiirítása
    StartGame(player, map, mapSize, playerPosition, boxPositions, targetPositions, boxCount); // Elindítja a játékot

    // Free allocated arrays
    FreeAllocatedMemoryFromMap(map); // Mátrix felszabadítása
    FreeDynamicArray(&boxPositions); // Doboz tömb felszabadítása
    FreeDynamicArray(&targetPositions); // Célmező tömb felszabadítása

}
void StartGame(Player *player, char ***map, Size mapSize, Point playerPosition, Point *boxPositions, Point *targetPositions, int boxCount){
    // Játék változói
    bool runGame = true;
    bool displayFrist = true;
    bool playerCanMove = false;
    Point direction;
    int key = -1;
    Point *Steps = &playerPosition;
    Point lastPoint;
//    Steps->next = NULL;
    // Játék Ciklusa
    while (runGame && !CheckWin(*map, mapSize)){
        // Wait for input
        key = econio_getch();
        // Evaluate input
        switch (key){
            case KEY_ESCAPE:
            case KEY_BACKSPACE:
                // Kilépés
                runGame = false;
                break;
            case KEY_UP:
                if (Move(map, mapSize, &playerPosition, &boxPositions, targetPositions, boxCount, up)){
                    playerCanMove = true;
                    direction = up;
                }
                break;
            case KEY_DOWN:
                if (Move(map, mapSize, &playerPosition, &boxPositions, targetPositions, boxCount, down)){
                    playerCanMove = true;
                    direction = down;
                }
                break;
            case KEY_LEFT:
                if (Move(map, mapSize, &playerPosition, &boxPositions, targetPositions, boxCount, left)){
                    playerCanMove = true;
                    direction = left;
                }
                break;
            case KEY_RIGHT:
                if (Move(map, mapSize, &playerPosition, &boxPositions, targetPositions, boxCount, right)){
                    playerCanMove = true;
                    direction = right;
                }
                break;
        }
        // Move Player
        if (playerCanMove){
            playerCanMove = false;
            //  Mező elhagyása
            char *lastCell = &(*map)[playerPosition.y][playerPosition.x]; // Az elhagyandó mező pointere
            if (*lastCell == PLAYER) *lastCell = EMPTY;
            else if (*lastCell == PLAYERONTARGET) *lastCell = TARGET;
            PrintPosition(*map, playerPosition);
            // Új mezőre lépés

            Point destinationPoint = AddPoints(playerPosition, direction);
            char *destinationCell = &(*map)[destinationPoint.y][destinationPoint.x]; // A lépendő mezőre mutató pointer
            if (*destinationCell == TARGET) *destinationCell = PLAYERONTARGET;
            else if (*destinationCell == EMPTY) *destinationCell = PLAYER;
            playerPosition = destinationPoint;
            PrintPosition(*map, playerPosition);
        }
        // Draw input to console

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
    Point destinationPoint = AddPoints(*currentPosition, direction);
    char *destinationCell = &((*map)[destinationPoint.y][destinationPoint.x]); // A lépendő mezőre mutató pointer. Így nem kell majd hosszú sort írni mindig
    //char *lastCell = &(*map)[currentPosition->y][currentPosition->x]; // Az elhagyandó mező pointere

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
            if (Move(map, mapSize, &destinationPoint, boxPositions, targetPositions, boxCount, direction)){
                int i = 0;
                while (!EqualToPoint((*boxPositions)[i],destinationPoint)){ i++; } // Aktuális helyen lévő doboz megkeresése
                //  Doboz Mező elhagyása
                //if (*lastCell == BOX) *lastCell = EMPTY;
                //else if (*lastCell == BOXONTARGET) *lastCell = TARGET;
                // Doboz Új mezőre léptetése
                destinationPoint = AddPoints(destinationPoint, direction); // Egyel odébb léptetjük, mert a dobozra fokuszálunk most
                destinationCell = &(*map)[destinationPoint.y][destinationPoint.x];
                if (*destinationCell == TARGET) *destinationCell = BOXONTARGET;
                else *destinationCell = BOX;
                *boxPositions[i] = destinationPoint;
                PrintPosition(*map, destinationPoint);
                return true;
            }
            else{
                return false;
            }
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
            printf("%s", chrWall);
            break;
        case TARGET:
            econio_textcolor(clrTarget);
            printf("%s", chrTarget);
            break;
        case PLAYER:
            econio_textcolor(clrPlayer);
            printf("%s", chrPlayer);
            break;
        case PLAYERONTARGET:
            econio_textcolor(clrPlayerOnTarget);
            econio_textbackground(clrTarget);
            printf("%s", chrPlayer);
            break;
        case BOX:
            econio_textcolor(clrBox);
            printf("%s", chrBox);
            break;
        case BOXONTARGET:
            econio_textcolor(clrBoxOnTarget);
            printf("%s", chrBox);
            break;
        default: printf(" "); break;
    }
}

// INIT: Játék előkészítéséhez szükséges függvények
void ReadXSBFile(char filename[], char ***map, Size *mapSize, Point *playerPosition, Point **boxPositions, Point **targetPositions, int *boxCount){
    *mapSize = (Size) { 0, 0}; // pálya méretének inicializálása
    int k = 0; // map index cikluscáltozója
    int boxindex = 0; // boxPositions tömb indexelésére szolgál
    int targetindex = 0; // célmezők tömb indexelésére szolgál
    //int boxCount = 0; // dobozok darabszáma == Célmezők darabszáma
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
        // oszlopok száma elhető legagyobb legyen, hogy minden sor elfréjen majd
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
                    *boxPositions[boxindex++] = (Point) {i, k};
                    break;
                case '*': // Box on target
                    *boxPositions[boxindex++] = (Point) {i, k};
                    *targetPositions[targetindex++] = (Point) {i, k};
                    break;
                case '+': // Player on target
                case '.': // Target
                    *targetPositions[targetindex++] = (Point) {i, k};
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
        free((*map)[0]);
        free(*map);
    }
}
void FreeDynamicArray(Point **boxPositions){
    if (*boxPositions != NULL){
        free(*boxPositions);
    }
}