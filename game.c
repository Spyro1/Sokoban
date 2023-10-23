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
    char **map; // Ez lesz a dinamikus tömb ami a pályát tárolja
    Size mapSize;
    //econio_clrscr();
    //debugmalloc_dump();
    // Új játék létrehozása
    NewGame(player, &map, mapSize,levelList[selectedLevel]);

    // Free map
    FreeAllocatedMemoryFromMap(&map);

}
void NewGame(Player *player, char ***map, Size mapSize, char levelName[]){
    ReadXSBFile(levelName, map, &mapSize);
    econio_rawmode();
    PrintSimpleMap(*map, mapSize);
    econio_getch();
}
void StartGame(Player *player, char ***map, Size mapSize){

}

void PrintSimpleMap(char **map, Size size){
    for(int y = 0; y < size.height; y++){
        for(int x = 0; x < size.width; x++){
            printf("%c", map[y][x]);
        }
        printf("\n");
    }
}
void PrintStyledMap(char **map, Size size, Point corner){

}

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
        strcpy((*map)[k], line);
        k++;
    }
    fclose(fp);
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