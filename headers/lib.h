#ifndef SOKOBAN2_LIB_H
#define SOKOBAN2_LIB_H

#include <stdbool.h>
#include "datatypes.h"

/**
 * Kiír a képernyőre egy figyelmeztető ablakot a megadott Message üzenettel, Igen/Nem válaszlehetőségekkel
 * @param Message Az üzenet
 * @param p képernyő közepének koordinátája
 * @param displayFirst Először megy-e be a ciklus a menüpontba tulajdonság (Cím szerint)
 * @param option  A kiválasztott opció
 * @param baseColor Alap betűszíne az ablaknak
 * @param accentForeColor Kijelölt opció betűszíne
 * @param accentBgColor Kijelölt opció háttérszíne
 */
void lib_WarningWindow(const char* Message, Point p, bool *displayFirst, int option, EconioColor baseColor, EconioColor accentForeColor, EconioColor accentBgColor);

/**
 * Letörli a cím alatt lévő területet a képernyőről
 */
void lib_ClearScrBellow();
/**
 * Letörli a képernyőt megadott koordinátákon belül a kapott bgColor színnel.
 * @param x1 Bal felső sarok x koordinátája
 * @param y1 Bal felső sarok y koordinátája
 * @param x2 Jobb alsó sarok x koordinátája
 * @param y2 Jobb alsó sarok y koordinátája
 * @param bgColor Törlendő terület háttérszíne
 */
void lib_ClearScreenSection(int x1, int y1, int x2, int y2, EconioColor bgColor);
/**
 * Kiírja képernyőre a hibaüzenetet
 * @param errormessage Hibaüzenet
 */
void lib_printError(const char* errormessage);
/**
 * Kiírja a képernyőre a kapott szöveget a megadott kezdő koordinátákra
 * @param str Kiírandó szöveg
 * @param x x koordináta a képernyőn
 * @param y y koordniáta a képernyőn
 */
void print(char const str[], int x, int y);
/**
 * Kiírja a képernyőre a kapott szöveget a megadott kezdő koordinátákra a megadott betűszínnel
 * @param str Kiírandó szöveg
 * @param x x koordináta a képernyőn
 * @param y y koordniáta a képernyőn
 * @param foreColor A szöveg színe
 */
void printfc(char const str[], int x, int y, EconioColor foreColor);
/**
 * Kiírja a képernyőre a kapott szöveget a megadott kezdő koordinátákra a megadott betűszínnel és háttérszínnel
 * @param str Kiírandó szöveg
 * @param x x koordináta a képernyőn
 * @param y y koordniáta a képernyőn
 * @param foreColor A szöveg színe
 * @param bgColor A szöveg háttérszíne
 */
void printfbc(char const str[], int x, int y, EconioColor foreColor, EconioColor bgColor);
/**
 * Megszámolja, hogy a kapott string hány valós karakterből áll, hány krakter íródik ki a képernyőre
 * @param str Karaktertömb, string (Bemenet)
 * @return A string hossza megjelenített karakterszámban
 */
int stringlenght(const char *str);
/**
 * Megszámolja, hogy hány byte-on tárolódik a max karakterszámú string
 * @param str Karaktertömb, string (Bemenet)
 * @param max Megjelenítendő karakterek száma
 * @return Megjelenítendő string max karakterű byte hossza
 */
int stringlenghtMax(const char *str, int max);
/**
 * Megnézi a függvény, hogy a string csak üres karaktereket tartalmaz-e (szóköz, \\n, \t)
 * @param str Karaktertömb, string (Bemenet)
 * @return Logikai: Csak üres karaktereket tartalmaz-e a string
 */
bool isBlankString(const char* str);

#endif //SOKOBAN2_LIB_H
