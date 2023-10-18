#ifndef SOKOBAN2_MAIN_H
#define SOKOBAN2_MAIN_H

#include <stdlib.h>
#include "econio.h"
#include "datatypes.h"

// Függvények
void MainScreen(Player **PlayerList, char *selectedLevelFileName);

void ReadPlayerTxtFile(Player **playerList, int *numOfPlayers);

void AddPlayer(Player *newPlayer, Player **playerList, int *numOfPlayers);

Player *MakePlayer(char name[], int completedLevels, int totalMoves, int averageMoves);

void FreePlayerList(Player *playerList);

void PrintPlayerList(Player *playerList, int numOfPlayers, int selectedPlayerIndex, Point start);

//void PrintMap(char const **map, int width, int height);


void ClearScreenSection(int x1, int y1, int x2, int y2, EconioColor bgColor);

void ReadDirectoryLevelNames(char directory[], char ***fileNames, int *numOfFiles);

void PrintLevels(char *levelList[], int numOfLevels, int selectedLevel, int maxLevels, Point start);

void ReadXSBFile(char const filename[], char ***map);

#endif
