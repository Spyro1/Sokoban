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

    char **map; // = (char*)malloc(totalHeight * totalWidth * sizeof(char)); // Ez lesz a dinamikus tömb ami a pályát tárolja

}
void NewGame(Player *player, char ***map, char levelName[]){
    ReadXSBFile(levelName, map);
}
void StartGame(Player *player, char ***map){

}

void PrintMap(char const **map, int width, int height){
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            printf("%c", map[y][x]);
        }
        printf("\n");
    }
}

void ReadXSBFile(char filename[], char ***map){
    int lines = 0, columns = 0;

    char directory[200] = "./levels/"; // Mappa
    strcat(directory, filename); // Mappa és fájlnév egyesítése
    FILE* fp = fopen(directory, "r"); // Fájl megnyitása
    if (fp == NULL) {
        printf("Nem sikerült megnyitni a fájlt: %s\n", filename);
        return;
    }
    // Sorok és oszlopok megszámlálása
    char line[maxLineLenght] = {0}; // Beolvasott sor a line stringbe megy
    while(fgets(line, maxLineLenght, fp)){ // Sor beolvasása
        lines++; // Ha sikeres, a sorokat egyel növeljük
        if (strlen(line) > columns) columns = (int)strlen(line) + 1; // oszlopok száma maximális legyen
    }
    rewind(fp);

    //    char **newmap;
    while(fgets(line, maxLineLenght, fp)){

    }
    fclose(fp);
}