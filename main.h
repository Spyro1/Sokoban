#ifndef SOKOBAN2_MAIN_H
#define SOKOBAN2_MAIN_H

#include <stdlib.h>
#include "econio.h"
#include "datatypes.h"
// Konstansok
#define activeForeColor COL_LIGHTCYAN
#define activeBgColor COL_RESET
#define baseForeColor COL_LIGHTBLUE



//Függvények
void MainScreen(Player *currentPlayer, int numOfLevels, int *selectedLevel);

void ClearScrBellow();
/**
 * Clears the screen section by the given background color with blank space within the given corners
 * @param x1 x coordinate of top left corner
 * @param y1 y coordinate of top left corner
 * @param x2 x coordinate of bottom right corner
 * @param y2 y coordinate of bottom right corner
 * @param bgColor background color of the section
 */
void ClearScreenSection(int x1, int y1, int x2, int y2, EconioColor bgColor);
/**
 * Prints the 'str' text to the console at the give position with base color
 * @param str string to display
 * @param x x coordinate on the console
 * @param y y coordinate on the console
 */
void print(char const str[], int x, int y);
/**
 * Prints the 'str' text to the console at the give position with the given foreground color.
 * @param str string to display
 * @param x x coordinate on the console
 * @param y y coordinate on the console
 * @param foreColor foreground color of the text
 */
void printfc(char const str[], int x, int y, EconioColor foreColor);
/**
 * Prints the 'str' text to the console at the give position with the given foreground and background colors.
 * @param str string to display
 * @param x x coordinate on the console
 * @param y y coordinate on the console
 * @param foreColor foreground color of the text
 * @param bgColor background color of the text
 */
void printfbc(char const str[], int x, int y, EconioColor foreColor, EconioColor bgColor);



#endif
