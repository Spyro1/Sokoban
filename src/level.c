#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include "../libraries/debugmalloc.h"
#include "../libraries/econio.h"
#include "../headers/level.h"
#ifdef _WIN32
    #include <windows.h>
#endif


void ReadDirectoryLevelNames(char directory[], char **levelList[], int *numOfFiles){
    DIR *folder = opendir(directory);
    struct dirent *dir;
    int count = 0;
    if(folder == NULL){
        printf("Nem lehet megnyitni a fájlt\n");
        perror("Unable to read directory");
    }
    while((dir=readdir(folder)))
    {
        if (dir->d_name[0] != '.'){
            count++;
            //printf("File %3d: %s\n", files, dir->d_name);
        }
    }
    rewinddir(folder);
    int i = 0;
    char **fileList = (char**) malloc(count * sizeof(char*));
    while((dir=readdir(folder)))
    {
        if (dir->d_name[0] != '.'){
            //printf("%d: %s\n",i, dir->d_name);
            fileList[i] = (char*) malloc(strlen(dir->d_name)+1);
            strcpy(fileList[i],dir->d_name);
            i++;
        }
    }
    *levelList = fileList;
    *numOfFiles = count;
    closedir(folder);
}
void FreeLevelList(char ***levelList, const int *numOfLevels){
    if (*levelList != NULL){
        for(int i = 0; i < *numOfLevels; i++)
        {
            free((*levelList)[i]);
        }
        free(*levelList);
        numOfLevels = 0;
    }
}
void PrintLevels(char **levelList, int numOfLevels, int selectedLevel, int maxLevels, Point start){
    int currentIndex = 0;
    while(currentIndex < maxLevels && currentIndex < numOfLevels) {
        econio_gotoxy(start.x, start.y + currentIndex);
        if (currentIndex == selectedLevel){
            econio_textcolor(COL_BLUE);
            econio_textbackground(COL_LIGHTCYAN);
        }
        else {
            econio_textcolor(COL_LIGHTCYAN);
            econio_textbackground(COL_RESET);
        }
        printf("%s", levelList[currentIndex]);
        currentIndex++;
        econio_textbackground(COL_RESET);
    }
}

