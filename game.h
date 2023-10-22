#ifndef GITIGNORE_GAME_H
#define GITIGNORE_GAME_H

#include "datatypes.h"
#include "econio.h"
#include "player.h"

/* == Konstansok == */
#define maxLineLenght 100

/*char chrPlayer[] = "◉", //◎,
        chrPlyerOnTarget[] = "◉",
        chrWall[] = "█",
        chrTarget[] = "◈",
        chrBox[] = "▢", //◾"//"◽""⊠",
        chrBoxOnTarget[] = "▣";
int  colorPlayer = COL_CYAN,
          colorPlayerOnTarget = COL_BLUE,
          colorBox = COL_BROWN,
          colorBoxOnTarget = COL_LIGHTGREEN,
          colorWall = COL_LIGHTGRAY,
          colorTarget = COL_RED;*/

void Init(Player *player, char *levelList[], int numOfLevels, int selectedLevel);

void NewGame(Player *player, char ***map, char levelName[]);

void StartGame(Player *player, char ***map);

void ReadXSBFile(char filename[], char ***map);

#endif //GITIGNORE_GAME_H
