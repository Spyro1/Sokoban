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


// = Függvények =

/**
 * Kiirítja a főcímet a képernyő tetejére nagy betűkkel több sorosan.
 */
static void menu_PrintTitle();
/**
 * Kiírja a képernyő aljára az aktuális menüpot nevigációs lehetőségeit.
 */
static void menu_PrintNavControls();

// = Almenü kiíró függvények =
/**
 * Kiír a képernyére egy ablakot, amiben megkérdezi a felhasználót, hogy biztos ki akar e lépni, Igen/Nem válaszlehetőségekkel.
 * @param p Point A kiíráshoz legfelső középső pont a képernyőn.
 */
static void menu_PrintExitWindow(Point p);
/**
 * Kiírja a képernyőre a főmenü menüpontjait, és kijelöli az aktuálisan kiválasztott menüpontot más színnel.
 * @param p Point A kiíráshoz legfelső középső pont a képernyőn.
 */
static void menu_PrintMainMenu(Point p);
/**
 * Kiírja a képernyőre az új játékos felvételéhez szükséges mezőt, és a bemenet után hozzáadja a játéklistához az új játékost.
 * @param playerListHead Player** A játékosok adatait tartalmazó láncolt lista. (Cím szerint)
 * @param numOfPlayers int* A játékosok darabszáma. (Cím szerint)
 * @param p Point A kiíráshoz legfelső középső pont a képernyőn.
 */
static void menu_PrintNewPlayerSubMenu(Player **playerListHead, int *numOfPlayers, Point p);
/**
 * Kiírja a képernyőre a játékoslistát, és kijelöli az aktuálisan kiválasztott játékost más színnel.
 * @param playerListHead Player** A játékosok adatait tartalmazó láncolt lista. (Cím szerint)
 * @param currentPlayer Player** A kiválasztott játékos struktúrájára mutató pointer, playerListHead elem. (Cím szerint)
 * @param numOfPlayers int* A játékosok darabszáma. (Cím szerint)
 * @param p Point A kiíráshoz legfelső középső pont a képernyőn.
 */
static void menu_PrintPlayerSubMenu(Player **playerListHead, Player **currentPlayer,  int *numOfPlayers, Point p );
/**
 * Kiírja a képernyőre a dicsőséglistát táblázatosan.
 * @param playerListHead Player** A játékosok adatait tartalmazó láncolt lista. (Cím szerint)
 * @param numOfPlayers int* A játékosok darabszáma. (Cím szerint)
 * @param p Point A kiíráshoz legfelső középső pont a képernyőn.
 */
static void menu_PrintRankList(Player **playerListHead, int *numOfPlayers, Point p);
/**
 * Kiírja a képernyőre, hogy teljesítette a szinteket a játékos.
 * @param p Point A kiíráshoz legfelső középső pont a képernyőn.
 */
static void menu_PrintWinGame(Point p);

/**
 * Visszaállítja a kezdőértékeket a menüben, ha menüpontváltás volt.
 */
static void menu_ResetMenuVars();
/**
 * Kiértékeli a felhasználó által lenyomott billentyőt, és megváltoztatja a program állapotát aszerint.
 * @param currentPlayer Player* A kiválasztott játékos struktúrájára mutató pointer, playerListHead elem.
 * @param playerListHead Player** A játékosok adatait tartalmazó láncolt lista. (Cím szerint)
 * @param numOfPlayers *char[] A játékosok darabszáma. (Cím szerint)
 * @param levelList int A szintek fájlnevét tároló dinamikus string tömb.
 */
static void menu_KeyPress(Player *currentPlayer, Player **playerListHead, int *numOfPlayers, char *levelList[], int numOfLevels);
/**
 * Az aktuális állapotnak megfelelően végrehajtja a szükséges utasításokat, és kiértékeli a bemeneteket.
 * @param playerListHead Player** A játékosok adatait tartalmazó láncolt lista. (Cím szerint)
 * @param numOfPlayers int* A játékosok darabszáma. (Cím szerint)
 * @param currentPlayer Player** A kiválasztott játékos struktúrájára mutató pointer, playerListHead elem.
 * @param p Point A kiíráshoz legfelső középső pont a képernyőn.
 * @param linesPrinted int* Az előző menüpontba a képernyőre írt sorok.
 */
static void menu_EvaluateState(Player **playerListHead, int *numOfPlayers, Player **currentPlayer, Point p, int *linesPrinted);

/**
 * A főmenüt futtató függvény. Egyszer hivandó meg a mainben.
 */
void menu_MainScreen();

#endif //SOKOBAN2_MENU_H
