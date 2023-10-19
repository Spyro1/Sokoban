#ifndef SOKOBAN2_PLAYER_H
#define SOKOBAN2_PLAYER_H
#include <stdio.h>
#include <string.h>
#include "datatypes.h"
#include "econio.h"
#include "main.h"
#include "debugmalloc.h"
#include <dirent.h>
#include <string.h>




void player_ReadTxtFile(Player **playerList, int *numOfPlayers);

void player_AddPlayer(Player *newPlayer, Player **playerList, int *numOfPlayers);

Player *player_MakePlayer(char name[], int completedLevels, int totalMoves, int averageMoves);

void player_FreePlayerList(Player **playerList);

void player_PrintPlayerList(Player *playerList, int numOfPlayers, int selectedPlayerIndex, Point start);

Player* player_GetSelectedPlayer(Player *playerList, int selectedPlayer);

#endif //SOKOBAN2_PLAYER_H
