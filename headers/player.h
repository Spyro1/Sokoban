#ifndef SOKOBAN2_PLAYER_H
#define SOKOBAN2_PLAYER_H
#include <stdio.h>
#include <string.h>
#include "datatypes.h"
#include "../libraries/econio.h"
#include "main.h"
#include "../libraries/debugmalloc.h"
#include <dirent.h>
#include <string.h>

#define playerDataPath "./players.txt"


void player_ReadTxtFile(Player **playerList, int *numOfPlayers);

void player_WriteTxtFile(Player *playerListHead, int numOfPlayers);

void player_FreePlayerList(Player **playerList);

void player_FreePlayerNode(Player **playerNode);

Player *player_MakePlayer(char name[], int numOfLevels, Statistics *statsListHead);

void player_AddPlayerToEnd(Player *newPlayer, Player **playerListHead, int *numOfPlayers);

bool player_RemovePlayer(Player *removablePlayer, Player **playerListHead, int *numOfPlayers);

void player_PrintPlayerList(Player *playerList, int selectedPlayerIndex, Point start);

void PrintRankList(Player *playerList, int numOfPlayer, Point p);

Player* player_GetSelectedPlayer(Player *playerList, int selectedPlayer);

int player_GetIndexOfPlayer(Player *playerListHead, char name[]);

#endif //SOKOBAN2_PLAYER_H
