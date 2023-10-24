#ifndef GITIGNORE_GAME_H
#define GITIGNORE_GAME_H

#include "datatypes.h"
#include "econio.h"
#include "player.h"


/* == Konstansok == */
// Karakterek
#define chrWall "█"
#define chrPlayer "¤"
#define chrTarget " "
#define chrBox "▢"

// Színek
#define clrWall COL_DARKGRAY
#define clrPlayer COL_CYAN
#define clrPlayerOnTarget COL_RED
#define clrTarget COL_RED
#define clrBox COL_BROWN
#define clrBoxOnTarget COL_GREEN


void Init(Player *player, char *levelList[], int numOfLevels, int selectedLevel);

void NewGame(Player *player, char ***map, Size mapSize, char levelName[]);

void StartGame(Player *player, char ***map, Size mapSize);

void ReadXSBFile(char filename[], char ***map, Size *mapSize);

char* ConvertInputLineToCellType(char line[]);

void PrintSimpleMap(char **map, Size size);

void PrintStyledMap(char **map, Size size, Point corner);
void PrintPosition(char **map, Point pos, Point corner);

void AllocateMemoryToMap(char ***map, Size *mapSize);

void FreeAllocatedMemoryFromMap(char ***map);

#endif //GITIGNORE_GAME_H
