#ifndef GITIGNORE_GAME_H
#define GITIGNORE_GAME_H

#include "econio.h"

/* == Konstansok == */

char const chrPlayer[] = "◉", //◎,
chrPlyerOnTarget[] = "◉",
        chrWall[] = "█",
        chrTarget[] = "◈",
        chrBox[] = "▢", //◾"//"◽""⊠",
chrBoxOnTarget[] = "▣";
int const colorPlayer = COL_CYAN,
          colorPlayerOnTarget = COL_BLUE,
          colorBox = COL_BROWN,
          colorBoxOnTarget = COL_LIGHTGREEN,
          colorWall = COL_LIGHTGRAY,
          colorTarget = COL_RED;

void StartGame(char ***map);
void ReadXSBFile(char const filename[], char ***map);

#endif //GITIGNORE_GAME_H
