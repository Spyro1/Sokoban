#ifndef SOKOBAN2_PLAYER_H
#define SOKOBAN2_PLAYER_H
#include <stdio.h>
#include <string.h>
#include "datatypes.h"
#include "../libraries/econio.h"
#include "../libraries/debugmalloc.h"
#include <dirent.h>
#include <string.h>

#define playerDataPath "./players.txt"

// == Fájlba Írás/Olvasás ==
/**
 * Beolvassa  a playerDataPath-ban megadott fájlt, és elátrolja a playerListHead láncolt listában
 * @param playerListHead A játékosok adatait tartalmazó láncolt lista (Cím szerint)
 * @param numOfPlayers A játékosok darabszáma (Cím szerint)
 */
void player_ReadTxtFile(Player **playerListHead, int *numOfPlayers);
/**
 * Kiírja fájlba a playeListHead-ben tárolt játékosok adatait: név;szintek;lépészámok..
 * @param playerListHead A játékosok adatait tartalmazó láncolt lista (Cím szerint)
 * @param numOfPlayers A játékosok darabszáma (Cím szerint)
 */
void player_WriteTxtFile(Player *playerListHead, int numOfPlayers);

// Memória foglalás és felszabadítás
/**
 * Létrehoz egy Player struktúrára mutató pointert a paraméterként kapott értékekből, hogy aztán Lístába lehessen fűzni.
 * @param name A játékos neve (max 20 karakter)
 * @param numOfLevels A játékos által teljesített szintek száma
 * @param statsListHead A játékos lépésstatisztikájának láncolt listája
 * @return Player struktúrűra mutató pointer a kapott adatokkal
 */
Player *player_MakePlayer(char name[], int numOfLevels, Statistics *statsListHead);
/**
 * Felszabadítja a az egész láncolt listának foglalt memóriát
 * @param playerListHead  * @param playerListHead A játékosok adatait tartalmazó láncolt lista (Cím szerint)
 */
void player_FreePlayerList(Player **playerListHead);
/**
 * Felszabadítja egy elem lefoglalt memóriáját a listából
 * @param playerNode Egy Player struktúrára mutató pointer a láncolt listából (Cím szerint)
 */
static void player_FreePlayerNode(Player **playerNode);

// Láncolt Lista függvények ==
/**
 *
 * @param newPlayer
 * @param playerListHead
 * @param numOfPlayers
 */
void player_AddPlayerToEnd(Player *newPlayer, Player **playerListHead, int *numOfPlayers);
/**
 *
 * @param newPlayer
 * @param playerListHead
 * @param numOfPlayers
 */
void player_AddPlayerInOrder(Player *newPlayer, Player **playerListHead, int *numOfPlayers);
/**
 *
 * @param removablePlayer
 * @param playerListHead
 * @param numOfPlayers
 * @return
 */
bool player_RemovePlayer(Player *removablePlayer, Player **playerListHead, int *numOfPlayers);
/**
 *
 * @param playerListHead
 * @param selectedPlayer
 * @return
 */
Player* player_GetSelectedPlayer(Player *playerListHead, int selectedPlayer);
/**
 *
 * @param playerListHead
 * @param name
 * @return
 */
int player_GetIndexOfPlayer(Player *playerListHead, char name[]);

// == Kiíratás ==
/**
 *
 * @param playerList
 * @param selectedPlayerIndex
 * @param start
 */
void player_PrintPlayerList(Player *playerList, int selectedPlayerIndex, Point start);

#endif //SOKOBAN2_PLAYER_H
