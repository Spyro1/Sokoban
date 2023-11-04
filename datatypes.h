#ifndef SOKOBAN2_DATATYPES_H
#define SOKOBAN2_DATATYPES_H
#include <stdbool.h>
#include "econio.h"

#define nameLenght 21
#define maxReadLineLenght 400 // 20 karakter név, 3karakter szint szám,
#define maxFileNameLenght 200
// == Adatstruktúrák ==

typedef enum celltype { null = '0', EMPTY = '_', WALL = 'W', TARGET = 'T', PLAYER = 'P', PLAYERONTARGET = 'p', BOX = 'B', BOXONTARGET = 'b'  } CellType;

typedef struct position{
    int x, y;
    struct Point *next;
} Point;
typedef struct size{
    int width, height;
} Size;
typedef struct celldata{
    enum celltype cell;
    int count;
} CellData;

typedef struct player {
    char name[nameLenght];
    int numOfCompletedLevels;
    int *levelMoves;
    struct Player *next;//, *back;
} Player;

// Functions

Point AddPoints(Point a, Point b);

Point SubPoints(Point a, Point b);

bool EqualToPoint(Point a, Point b);

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

#endif //SOKOBAN2_DATATYPES_H
