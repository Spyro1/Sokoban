#ifndef SOKOBAN2_LIB_H
#define SOKOBAN2_LIB_H

#include <stdbool.h>
#include "datatypes.h"

/**
 * Kiír a képernyőre egy figyelmeztető ablakot a megadott Message üzenettel, Igen/Nem válaszlehetőségekkel
 * @param Message char[] Az üzenet
 * @param p Point képernyő közepének koordinátája
 * @param displayFirst bool* Először megy-e be a ciklus a menüpontba tulajdonság (Cím szerint)
 * @param option int A kiválasztott opció
 * @param baseColor EconioColor Alap betűszíne az ablaknak
 * @param accentForeColor EconioColor Kijelölt opció betűszíne
 * @param accentBgColor EconioColor Kijelölt opció háttérszíne
 */
void lib_WarningWindow(const char Message[], Point p, bool *displayFirst, int option, EconioColor baseColor, EconioColor accentForeColor, EconioColor accentBgColor);

/**
 * Letörli a cím alatt lévő területet a képernyőről
 */
void lib_ClearScrBellow();
/**
 * Letörli a képernyőt megadott koordinátákon belül a kapott bgColor színnel.
 * @param x1 int Bal felső sarok x koordinátája
 * @param y1 int Bal felső sarok y koordinátája
 * @param x2 int Jobb alsó sarok x koordinátája
 * @param y2 int Jobb alsó sarok y koordinátája
 * @param bgColor EconioColor Törlendő terület háttérszíne
 */
void lib_ClearScreenSection(int x1, int y1, int x2, int y2, EconioColor bgColor);
/**
 * Kiírja képernyőre a hibaüzenetet
 * @param errormessage char[] Hibaüzenet
 */
void lib_printError(const char errormessage[]);
/**
 * Kiírja a képernyőre a kapott szöveget a megadott kezdő koordinátákra
 * @param str char[] Kiírandó szöveg
 * @param x int x koordináta a képernyőn
 * @param y int y koordniáta a képernyőn
 */
void print(char const str[], int x, int y);
/**
 * Kiírja a képernyőre a kapott szöveget a megadott kezdő koordinátákra a megadott betűszínnel
 * @param str char[] Kiírandó szöveg
 * @param x int x koordináta a képernyőn
 * @param y int y koordniáta a képernyőn
 * @param foreColor EconioColor A szöveg színe
 */
void printfc(char const str[], int x, int y, EconioColor foreColor);
/**
 * Kiírja a képernyőre a kapott szöveget a megadott kezdő koordinátákra a megadott betűszínnel és háttérszínnel
 * @param str char[] Kiírandó szöveg
 * @param x int x koordináta a képernyőn
 * @param y int y koordniáta a képernyőn
 * @param foreColor EconioColor A szöveg színe
 * @param bgColor EconioColor A szöveg háttérszíne
 */
void printfbc(char const str[], int x, int y, EconioColor foreColor, EconioColor bgColor);
/**
 * Két pont koordinátáit összeadó függvény
 * @param a Point Egyik koordináta
 * @param b Point Másik koordináta
 * @return Point Koordináták összege
 */
Point addPoints(Point a, Point b);
/**
 * Két pont koordinátáinak különbségét kiszámoló függvény
 * @param a Point Egyik koordináta
 * @param b Point Másik koordniáta
 * @return Point Koordináták különbsége
 */
Point subPoints(Point a, Point b);
/**
 * Két pontot hasonlít össze, hogy egyenlőek-e
 * @param a Point Egyik koordináta
 * @param b Point Másik koordináta
 * @return bool Egyenlőek e a paraméterként kapott koordináták
 */
bool comparePoints(Point a, Point b);

/**
 * Megszámolja, hogy a kapott string hány valós karakterből áll, hány krakter íródik ki a képernyőre
 * @param str char[] Karaktertömb, string (Bemenet)
 * @return int A string hossza megjelenített karakterszámban
 */
int stringlenght(const char str[]);
/**
 * Megszámolja, hogy hány byte-on tárolódik a max karakterszámú string
 * @param str char[] Karaktertömb, string (Bemenet)
 * @param max int Megjelenítendő karakterek száma
 * @return int Megjelenítendő string max karakterű byte hossza
 */
int stringlenghtMax(const char str[], int max);
/**
 * Megnézi a függvény, hogy a string csak üres karaktereket tartalmaz-e (szóköz, \\n, \t)
 * @param str char[] Karaktertömb, string (Bemenet)
 * @return bool Csak üres karaktereket tartalmaz-e a string
 */
bool isBlankString(const char* str);

#endif //SOKOBAN2_LIB_H
