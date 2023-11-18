#ifndef SOKOBAN2_LIB_H
#define SOKOBAN2_LIB_H

#include <stdbool.h>
#include "datatypes.h"


void lib_WarningWindow(const char* Message, Point p, bool *displayFirst, int option, EconioColor baseColor, EconioColor accentForeColor, EconioColor accentBgColor);

/* Letörli a cím alatt lévő területet a képernyőről */
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

int stringlenght(const char *s);
int stringlenghtMax(const char *s, int max);
bool isBlankString(const char* str);
//char *cutAndFixString(const char *str);

#endif //SOKOBAN2_LIB_H
