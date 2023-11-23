#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include "../libraries/debugmalloc.h"
#include "../headers/level.h"
#include "../headers/lib.h"


void level_ReadDirectory(char directory[], char **levelList[], int *numOfFiles) {
    DIR *folder = opendir(directory); // Mappa megnyitása
    struct dirent *dir;
    int count = 0;
    if(folder == NULL){
        lib_printError("Nem lehet megnyitni a ./levels/ mappát");
    }
    // Megszámolja hány fájl van a mappában
    while((dir=readdir(folder)))
    {
        if (dir->d_name[0] != '.'){
            count++;
        }
    }
    rewinddir(folder);
    int i = 0;
    // Lefoglal egy dinamikus string tömböt a szintek fájlnevének
    char **fileList = (char**) malloc(count * sizeof(char*));
    // fájlnevek beolvasása
    while((dir=readdir(folder)))
    {
        if (dir->d_name[0] != '.'){
            fileList[i] = (char*) malloc(strlen(dir->d_name)+1);
            strcpy(fileList[i],dir->d_name);
            i++;
        }
    }
    // Értékek visszaadása
    *levelList = fileList;
    *numOfFiles = count;
    closedir(folder);
}
void level_FreeLevelList(char **levelList[], int *numOfLevels) {
    if (*levelList != NULL){
        for(int i = 0; i < *numOfLevels; i++)
        {
            free((*levelList)[i]);
        }
        free(*levelList);
        *numOfLevels = 0;
    }
}
