#ifndef GITIGNORE_GAME_H
#define GITIGNORE_GAME_H

/* == Konstansok == */

char const chrPlayer[] = "◉", //◎,
chrPlyerOnTarget[] = "◉",
        chrWall[] = "█",
        chrTarget[] = "◈",
        chrBox[] = "▢", //◾"//"◽""⊠",
chrBoxOnTarget[] = "▣";

void StartGame(char ***map);


#endif //GITIGNORE_GAME_H
