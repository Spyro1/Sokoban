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
    char **map; // Pálya
    Size mapSize; // Pálya mérete

    // Új játék létrehozása
    NewGame(player, &map, mapSize,levelList[selectedLevel]);
    // Free map
    FreeAllocatedMemoryFromMap(&map);

}
void NewGame(Player *player, char ***map, Size mapSize, char levelName[]){
    ReadXSBFile(levelName, map, &mapSize);
    econio_rawmode();
    //PrintSimpleMap(*map, mapSize);
    PrintStyledMap(*map,mapSize,(Point) {5,5});
    econio_getch();
}
void StartGame(Player *player, char ***map, Size mapSize){

}

//void Step(int key)

// Konzolra Írás
void PrintSimpleMap(char **map, Size size){
    for(int y = 0; y < size.height; y++){
        for(int x = 0; x < size.width; x++){
            printf("%d", map[y][x]);
        }
        printf("\n");
    }
}
void PrintStyledMap(char **map, Size size, Point corner){
    for(int y = 0; y < size.height; y++){
        for(int x = 0; x < size.width; x++){
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
void ReadXSBFile(char filename[], char ***map, Size *mapSize){
    *mapSize = (Size) { 0, 0};
    int k = 0;
    char directory[200] = "./levels/"; // Mappa
    strcat(directory, filename); // Mappa és fájlnév egyesítése
    FILE* fp = fopen(directory, "r"); // Fájl megnyitása
    if (fp == NULL) {
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
    }
    rewind(fp);
    //fclose(fp);
    //fp = fopen(directory, "r"); // Fájl megnyitása
    //debugmalloc_dump();
    // Tömb foglalása
    FreeAllocatedMemoryFromMap(map);
    AllocateMemoryToMap(map, mapSize);

    // Tömbbe olvasás
    while(fgets(line, maxReadLineLenght, fp)){
        char* converted = ConvertInputLineToCellType(line);
        strcpy((*map)[k], converted);
        k++;
    }
    fclose(fp);
}
char* ConvertInputLineToCellType(char line[]){
    int lenght = (int)strlen(line);
    //char converted[maxReadLineLenght] = {0};
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
    newMap = (char**) malloc(mapSize->height * sizeof(char*)); // itt a hiba
    if (newMap == NULL) { perror("Nem sikerült memóriaterületet foglalni newMap-nek"); return; }
    newMap[0] = (char*) malloc((mapSize->height * mapSize->width) * sizeof(char));
    for (int y = 1; y < mapSize->height; y++){
        newMap[y] = newMap[0] + y * mapSize->width;
    }
    *map = newMap;
}
void FreeAllocatedMemoryFromMap(char ***map){
    if (*map != NULL) {
        free((*map)[0]);
        free(*map);
    }
}