#ifndef GITIGNORE_GAME_H
#define GITIGNORE_GAME_H

#include "datatypes.h"
#include "econio.h"
#include "player.h"


/* == Konstansok == */
// Karakterek
#define chrWall "█"
#define chrPlayer "☻" //◯
#define chrTarget "◻" // "◻□◼
#define chrBox "◼" //▩◼⛝ //  ▩◼⛝ ◻□◼

// Színek
#define clrWall COL_DARKGRAY
#define clrPlayer COL_CYAN
#define clrPlayerOnTarget COL_RED
#define clrTarget COL_LIGHTRED
#define clrBox COL_BROWN
#define clrBoxOnTarget COL_GREEN




bool Init(Player *player, char **levelList, int numOfLevels);

bool StartGame(Player *player, char levelName[]);

bool CheckWin(char **map, Size mapSize);

bool Move(char ***map, Size mapSize, Point *currentPosition, Point **boxPositions, Point *targetPositions, int boxCount, Point direction);

void ReadXSBFile(char filename[], char ***map, Size *mapSize, Point *playerPosition, Point **boxPositions, Point **targetPositions, int *boxCount);

char* ConvertInputLineToCellType(char line[]);

void PrintSimpleMap(char **map, Size size);

void PrintStyledMap(char **map, Size size);
void PrintPosition(char **map, Point pos);

void AllocateMemoryToMap(char ***map, Size *mapSize);
void AllocateDynamicArray(Point **newArray, int lenght);
void FreeAllocatedMemoryFromMap(char ***map);
void FreeDynamicArray(Point **dynamicArray);

#endif //GITIGNORE_GAME_H
