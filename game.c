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

#ifdef _WIN32
#include <windows.h>
#endif

void StartGame(Player *player, char *levelFileName){
    //char **map; // = (char*)malloc(totalHeight * totalWidth * sizeof(char)); // Ez lesz a dinamikus tömb ami a pályát tárolja
    econio_clrscr();
    int key = -1;
    printf("Játék kezdete");
    key = econio_getch();
}

/*void PrintMap(char const **map, int width, int height){
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            printf("%c", map[y][x]);
        }
        printf("\n");
    }
}*/

void ReadXSBFile(char const filename[], char ***map){
//    char **newmap;
//    FILE* fp = fopen(filename, "r");
// foglalás
//    newmap = (char**) malloc(magassag * sizeof(char*));
//    for (int y = 0; y < magassag; ++y)
//        szamok[y] = (char*) malloc(szelesseg * sizeof(char));

}