#ifndef SOKOBAN2_MENU_H
#define SOKOBAN2_MENU_H

//#include <stdlib.h>
#include "../libraries/econio.h"
#include "datatypes.h"

// Makrók
#define strNewPlayer "ÚJ JÁTÉKOS"
#define strChosePlayer "JÁTÉK INDÍTÁSA"
#define strRankList "DICSŐSÉGLISTA"
#define strExitApp "KILÉPÉS"
#define strNewPlayerTitle "= ÚJ JÁTÉKOS FELVÉTELE ="
#define strEditPlayerTitle "= JÁTÉKOSNÉV SZERKESZTÉS ="
#define strChosePlayerTitle "= JÁTÉKOSOK ="
#define strRankListTitle "= DICSŐSÉGLISTA ="


#define waitsec 0.6
#define maxDisplayLines 10


// Függvények


/**
 * Kiirítja a főcímet a képernyő tetejére nagy betűkkel több sorosan
 */
static void menu_PrintTitle();
/**
 * Kiírja a képernyő aljára az aktuális menüpot nevigációs lehetőségeit
 */
static void menu_PrintNavControls();

// = Almenü kiíró függvények =
static void menu_PrintExitWindow(Point p);
static void menu_PrintMainMenu(Point p);
static void menu_PrintNewPlayerSubMenu(Player **playerListHead, int *numOfPlayers, Point p);
static void menu_PrintPlayerSubMenu(Player **playerListHead, Player **currentPlayer,  int *numOfPlayers, Point p );
static void menu_PrintRankList(Player **playerListHead, int *numOfPlayers, Point p);
static void menu_PrintWinGame(Point p);

/**
 * Visszaállítja a kezdőértékeket a menüben, ha menüpont váltás volt
 *
 */
static void menu_ResetMenuVars();
/**
 * Kiértékeli a felhasználó által lenyomott billentyőt, és megváltoztatja a program állapotát aszerint
 * @param currentPlayer A kiválasztott játékos struktúrájára mutató pointer, playerList elem
 * @param playerListHead A játékosok adatait tartalmazó láncolt lista (Cím szerint)
 * @param numOfPlayers  A játékosok darabszáma (Cím szerint)
 * @param levelList A szintek fájlnevét tároló dinamikus string tömb
 */
static void menu_KeyPress(Player *currentPlayer, Player **playerListHead, int *numOfPlayers, char **levelList, int numOfLevels);
/**
 * Az aktuális állapotnak megfelelően végrehajtja a szükséges utasításokat, és kiértékeli a bemeneteket
 * @param playerListHead A játékosok adatait tartalmazó láncolt lista (Cím szerint)
 * @param currentPlayer  A kiválasztott játékos struktúrájára mutató pointer, playerList elem
 * @param numOfPlayers A játékosok darabszáma (Cím szerint)
 * @param p A kiíráshoz legfelső középső pont a képernyőn
 * @param linesPrinted Az előző menüpontba a képernyőre írt sorok száma
 */
static void menu_EvaluateState(Player **playerListHead, int *numOfPlayers, Player **currentPlayer, Point p, int *linesPrinted);

/**
 * A főmenüt futtató függvény. Egyszer hivandó meg a mainben
 */
void menu_MainScreen();

#endif //SOKOBAN2_MENU_H
