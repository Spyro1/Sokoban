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

// == Memória foglalás és felszabadítás ==
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
 * @param playerListHead A játékosok adatait tartalmazó láncolt lista (Cím szerint)
 */
void player_FreePlayerList(Player **playerListHead);
/**
 * Felszabadítja egy elem lefoglalt memóriáját a listából
 * @param playerNode Egy Player struktúrára mutató pointer a láncolt listából (Cím szerint)
 */
static void player_FreePlayerNode(Player **playerNode);

// == Láncolt Lista függvények ==
/**
 * Beszúrja a játékoslistának a végére az új játékos elemet
 * @param newPlayer Új játékos struktúrájára mutató pointer
 * @param playerListHead A játékosok adatait tartalmazó láncolt lista (Cím szerint)
 * @param numOfPlayers A játékosok darabszáma (Cím szerint)
 */
void player_AddPlayerToEnd(Player *newPlayer, Player **playerListHead, int *numOfPlayers);
/**
 * Beszúrja a játékoslistába au új játékost a nevének a hossza szerint növekvő sorrendben
 * @param newPlayer Új játékos struktúrájára mutató pointer
 * @param playerListHead A játékosok adatait tartalmazó láncolt lista (Cím szerint)
 * @param numOfPlayers A játékosok darabszáma (Cím szerint)
 */
void player_AddPlayerInOrder(Player *newPlayer, Player **playerListHead, int *numOfPlayers);
/**
 * Törli a paramterként kapott játékost a listából
 * @param removablePlayer A törlendő játékos struktúrájára mutató pointer
 * @param playerListHead A játékosok adatait tartalmazó láncolt lista (Cím szerint)
 * @param numOfPlayers A játékosok darabszáma (Cím szerint)
 * @return Logikai: Igaz, ha sikeres a törlés a listából; Hamis, ha nem sikerült törölni a játékost
 */
bool player_RemovePlayer(Player *removablePlayer, Player **playerListHead, int *numOfPlayers);
/**
 * Megkeresi a listában a selectedPlayer-edik elemet
 * @param playerListHead A játékosok adatait tartalmazó láncolt lista
 * @param selectedPlayer A játékos sorszáma / indexe a listában
 * @return A keresett játékos struktúrájára mutató pointer, ha megtalálta, különben NULL pointer 
 */
Player* player_GetSelectedPlayer(Player *playerListHead, int selectedPlayer);
/**
 * Megkeresi a listában a játékos nevét, és visszaadja a sorszámát / indexét  a listában
 * @param playerListHead A játékosok adatait tartalmazó láncolt lista
 * @param name A keresett játékos neve
 * @return A keresett játékos indexe, ha megtalálta, különben -1
 */
int player_GetIndexOfPlayer(Player *playerListHead, char name[]);

// == Kiíratás ==
/**
 * Kiírja a képernyőre a játékoslistát (nevüket és szintjüket) egymás alá, és kiemeli az aktuálisan kiválasztott játékost
 * @param playerList A játékosok adatait tartalmazó láncolt lista
 * @param selectedPlayerIndex Az aktuálisan kiválasztott játékos sorszáma / indexe
 * @param p A kiíráshoz legfelső középső pont a képernyőn
 */
void player_PrintPlayerList(Player *playerList, int selectedPlayerIndex, Point p);

#endif //SOKOBAN2_PLAYER_H
