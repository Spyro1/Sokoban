#ifndef GITIGNORE_GAME_H
#define GITIGNORE_GAME_H

#include "datatypes.h"
#include "econio.h"
#include "player.h"


/* == Konstansok == */
// Karakterek
#define chrWall "█"
#define chrPlayer "☻" //◯
#define chrTarget "◻"
#define chrBox "◼" //▩◼⛝

// Színek
#define clrWall COL_DARKGRAY
#define clrPlayer COL_CYAN
#define clrPlayerOnTarget COL_RED
#define clrTarget COL_LIGHTRED
#define clrBox COL_BROWN
#define clrBoxOnTarget COL_GREEN


void Init(Player *player, char *levelList[], int numOfLevels, int selectedLevel);

void NewGame(Player *player, char ***map, Size mapSize, char levelName[]);

void StartGame(Player *player, char ***map, Size mapSize, Point playerPosition, Point *boxPositions);

bool CheckWin(char **map, Size mapSize);

void ReadXSBFile(char filename[], char ***map, Size *mapSize, Point *playerPosition, Point **boxPositions);

char* ConvertInputLineToCellType(char line[]);

void PrintSimpleMap(char **map, Size size);

void PrintStyledMap(char **map, Size size, Point corner);
void PrintPosition(char **map, Point pos, Point corner);

void AllocateMemoryToMap(char ***map, Size *mapSize);
void AllocateMemoryToBoxPositions(Point **boxPositions, int lenght);
void FreeAllocatedMemoryFromMap(char ***map);
void FreeAllocatedMemoryFromBoxPositions(Point **boxPositions);

#endif //GITIGNORE_GAME_H
