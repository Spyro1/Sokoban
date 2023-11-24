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
 * A főmenüt futtató függvény. Egyszer hivandó meg a mainben
 */
void menu_MainScreen();

/**
 * Kiirítja a főcímet a képernyő tetejére nagy betűkkel több sorosan
 */
// void menu_PrintTitle();
/**
 * Kiértékeli a felhasználó által lenyomott billentyőt, és megváltoztatja a program állapotát aszerint
 * @param key A lenyomott billentyű kódja
 * @param state A program állapotát tároló érték (Cím szerint)
 * @param displayFirst Először megy-e be a ciklus a menüpontba tulajdonság (Cím szerint)
 * @param option  A kiválasztott opció (Cím szerint)
 * @param selectedPlayer A kiválasztott játékos indexének változója (Cím szerint)
 * @param runMenu Menü futtatását beállító logikai változó (Cím szerint)
 * @param currentPlayer A kiválasztott játékos struktúrájára mutató pointer, playerList elem
 * @param playerListHead A játékosok adatait tartalmazó láncolt lista (Cím szerint)
 * @param numOfPlayers  A játékosok darabszáma (Cím szerint)
 * @param levelList A szintek fájlnevét tároló dinamikus string tömb
 */
// void menu_KeyPress(int key, State *state, bool *displayFirst, int *option, int *selectedPlayer, bool *runMenu, Player *currentPlayer, Player **playerListHead, int *numOfPlayers, char **levelList, int numOfLevels);
/**
 * Az aktuális állapotnak megfelelően végrehajtja a szükséges utasításokat, és kiértékeli a bemeneteket
 * @param key A lenyomott billentyű kódja (Cím szerint)
 * @param state A program állapotát tároló érték (Cím szerint)
 * @param runMenu Fut-e a menü
 * @param displayFirst Először megy-e be a ciklus a menüpontba tulajdonság (Cím szerint)
 * @param option A kiválasztott opció
 * @param prevOption Az előző kiválasztott opció
 * @param playerListHead A játékosok adatait tartalmazó láncolt lista (Cím szerint)
 * @param currentPlayer  A kiválasztott játékos struktúrájára mutató pointer, playerList elem
 * @param numOfPlayers A játékosok darabszáma (Cím szerint)
 * @param selectedPlayer Az aktuálisan kiválasztott játékos indexe
 * @param p A kiíráshoz legfelső középső pont a képernyőn
 * @param linesPrinted Az előző menüpontba a képernyőre írt sorok száma
 */
// void menu_EvaluateState(int *key, State *state, bool runMenu, bool *displayFirst, int option, int prevOption, Player **playerListHead, Player **currentPlayer, int *numOfPlayers, int selectedPlayer, Point p, int *linesPrinted);

// = Almenü kiíró függvények =
// void menu_PrintExitWindow(bool runMenu, bool *displayFirst, int option, Point p);
// void menu_PrintMainMenu(bool *displayFirst, int option, int prevOption, Point p);
// void menu_PrintNewPlayerSubMenu(State *state, Player **playerListHead, int *numOfPlayers, int selectedPlayer, Point p);
// void menu_PrintPlayerSubMenu(bool *displayFirst, Player **playerListHead, int *numOfPlayers, Player **currentPlayer, int selectedPlayer, Point p );
// void menu_PrintRankList(bool *displayFirst, Player **playerListHead, int *numOfPlayers, Point p);
// void menu_PrintWinGame(State *state, bool *displayFirst, Point p);

/**
 * Visszaállítja a kezdőértékeket a menüben, ha menüpont váltás volt
 * @param displayFirst bool* Először megy-e be a ciklus a menüpontba tulajdonság
 * @param option int* A kiválasztott opciók változója
 * @param selectedPlayer int* A kiválasztott játékos indexének változója
 */
// void menu_ResetMenuVars(bool *displayFirst, int *option, int *selectedPlayer);
/**
 * Kiírja a képernyő aljára az aktuális menüpot nevigációs lehetőségeit
 * @param displayFirst bool Először megy-e be a ciklus a menüpontba tulajdonság
 * @param state State A program állapotát tároló érték
 */
// void menu_PrintNavControls(bool displayFirst, State state);
#endif //SOKOBAN2_MENU_H
