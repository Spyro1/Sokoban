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


void Init(Player *player, char *levelList[], int numOfLevels, int selectedLevel){
    // Fő változók deklarálása
    char **map = NULL; // Pálya
    Size mapSize; // Pálya mérete

    // Új játék létrehozása
    NewGame(player, &map, mapSize,levelList[selectedLevel]);
    // Free map
    FreeAllocatedMemoryFromMap(&map);

}
void NewGame(Player *player, char ***map, Size mapSize, char levelName[]){
    Point playerPosition;
    Point *boxPositions = NULL;
    ReadXSBFile(levelName, map, &mapSize, &playerPosition, &boxPositions); // map-be beolvassa a kapott levelName-t fájlból

    econio_rawmode(); // Billentyű módba kapcsolás
    PrintStyledMap(*map,mapSize,(Point) {5,5}); // Pálya kiirítása
    StartGame(player, map, mapSize, playerPosition, boxPositions); // Elindítja a játékot

    FreeAllocatedMemoryFromMap(map); // Játék végén felszabadítom a mátrixot
    FreeAllocatedMemoryFromBoxPositions(&boxPositions);

}
void StartGame(Player *player, char ***map, Size mapSize, Point playerPosition, Point *boxPositions){
    // Játék változói
    bool runGame = true;
    bool displayFrist = true;
    int key = -1;
    Point changedPos;
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

                break;
            case KEY_DOWN:

                break;
            case KEY_LEFT:

                break;
            case KEY_RIGHT:

                break;
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
// Konzolra Írás
void PrintSimpleMap(char **map, Size mapSize){
    for(int y = 0; y < mapSize.height; y++){
        for(int x = 0; x < mapSize.width; x++){
            printf("%d", map[y][x]);
        }
        printf("\n");
    }
}
void PrintStyledMap(char **map, Size mapSize, Point corner){
    for(int y = 0; y < mapSize.height; y++){
        for(int x = 0; x < mapSize.width; x++){
            PrintPosition(map, (Point) {x,y}, corner);
        }
    }
}
void PrintPosition(char **map, Point pos, Point corner){
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
            //econio_textbackground(clrTarget);
            printf("%s", chrBox);
            break;
        default: printf(" "); break;
    }
}

// INIT: Játék előkészítéséhez szükséges függvények
void ReadXSBFile(char filename[], char ***map, Size *mapSize, Point *playerPosition, Point **boxPositions){
    *mapSize = (Size) { 0, 0}; // pálya méretének inicializálása
    int k = 0; // map index cikluscáltozója
    int boxindex = 0; // boxPositions tömb indexelésére szolgál
    int boxCount = 0; // dobozok darabszáma == Célmezők darabszáma
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
                    boxCount++; // Doboz és célmezők számának növelése
                    break;
                default:
                    break;
            }
        }
        characterPtr = strchr(line, '@'); // Játékos karaktert van e a sorban

        // Player pozíció meghatározása
        //if (characterPtr != NULL) *playerPosition = (Point) {(int)(line - characterPtr), mapSize->height - 1 };
        //characterPtr = strchr(line, '$') == NULL ? strchr(line, '*') : strchr(line, '$');
        //if (characterPtr != NULL) boxCount++; // Boxokat megszámolja
    }
    rewind(fp);

    // Pálya Mátrix felszabadítása és foglalása
    FreeAllocatedMemoryFromMap(map);
    AllocateMemoryToMap(map, mapSize);
    // Dobozok tömb felszabadítása és foglalása
    FreeAllocatedMemoryFromBoxPositions(boxPositions);
    AllocateMemoryToBoxPositions(boxPositions, boxCount+1);

    // Tömbbe olvasás
    while(fgets(line, maxReadLineLenght, fp)){
        // Dobozok kordinátáinak eltárolása
        characterPtr = strchr(line, '$') == NULL ? strchr(line, '*') : strchr(line, '$');
        if (characterPtr != NULL) *boxPositions[boxindex++] = (Point) {(int)(line-characterPtr), k};
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
void AllocateMemoryToBoxPositions(Point **boxPositions, int lenght){
    Point *newList;
    newList = (Point*) malloc(lenght * sizeof(Point)); // Dobozok tömbjének memóriafoglalása
    if(newList == NULL) {
        perror("Nem sikerult memoriat foglalni a boxPositions tömbnek.");
        return;
    } // Hibakezelés
    *boxPositions = newList; // Címátadás
}
void FreeAllocatedMemoryFromMap(char ***map){
    if (*map != NULL) {
        free((*map)[0]);
        free(*map);
    }
}
void FreeAllocatedMemoryFromBoxPositions(Point **boxPositions){
    if (*boxPositions != NULL){
        free(*boxPositions);
    }
}