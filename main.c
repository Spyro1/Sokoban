#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include "econio.h"

#ifdef _WIN32
    #include <windows.h>
#endif


/* == Adatstruktúrák == */

typedef struct position{
    int x, y;
} pos;
typedef enum object { PLAYER, AIR, WALL, BOX, TARGET, EMPTY } object;


/* == Konstansok == */

char const chrPlayer[] = "◎",
           chrPlyerOnTarget[] = "◉",
           chrWall[] = "█",
           chrTarget[] = "◈",
           chrBox[] = "⊠",
           chrBoxOnTarget[] = "▣";
int const colorPlayer = COL_CYAN,
          colorPlayerOnTarget = COL_BLUE,
          colorBox = COL_BROWN,
          colorBoxOnTarget = COL_LIGHTGREEN,
          colorWall = COL_LIGHTGRAY,
          colorTarget = COL_RED,
          maxDificulty = 16,
          minDificulty = 1;





// Függvények
void MainScreen(int *selectedDificulty, int *selectedLevel, char *filename[]);
void PrintMap(char const **map, int width, int height);
void ReadXSBFile(char const filename[], char ***map);
int ToIndex(pos p);
pos ToPos(int index);


int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    // == Változók ==
    int totalWidth = 25,  // Szélesség
        totalHeight = 25; // Magasság
    pos playerPos; // A Játékos poziciója

    // Főképernyő kiiratása és Szint választás
    int selectedDificulty = 1,
        selectedLevel = 1;
    char selectedLevelFileName[100];
    MainScreen(&selectedDificulty, &selectedLevel, &selectedLevelFileName);

    char **map; // = (char*)malloc(totalHeight * totalWidth * sizeof(char)); // Ez lesz a dinamikus tömb ami a pályát tárolja
    ReadXSBFile(selectedLevelFileName, &map);
    return 0;
}
void MainScreen(int *selectedDificulty, int *selectedLevel, char *filename[]){
    // CÍM Kiírása
    /*printf("\n"
           "#   _______  _______  ___   _  _______  _______  _______  __    _ \n"
           "#  |       ||       ||   | | ||       ||  _    ||   _   ||  |  | |\n"
           "#  |  _____||   _   ||   |_| ||   _   || |_|   ||  |_|  ||   |_| |\n"
           "#  | |_____ |  | |  ||      _||  | |  ||       ||       ||       |\n"
           "#  |_____  ||  |_|  ||     |_ |  |_|  ||  _   | |       ||  _    |\n"
           "#   _____| ||       ||    _  ||       || |_|   ||   _   || | |   |\n"
           "#  |_______||_______||___| |_||_______||_______||__| |__||_|  |__|");*/
    /*printf("\n"
           " ________  ________  ___  __    ________  ________  ________  ________      \n"
           "|\\   ____\\|\\   __  \\|\\  \\|\\  \\ |\\   __  \\|\\   __  \\|\\   __  \\|\\   ___  \\    \n"
           "\\ \\  \\___|\\ \\  \\|\\  \\ \\  \\/  /|\\ \\  \\|\\  \\ \\  \\|\\ /\\ \\  \\|\\  \\ \\  \\\\ \\  \\   \n"
           " \\ \\_____  \\ \\  \\\\\\  \\ \\   ___  \\ \\  \\\\\\  \\ \\   __  \\ \\   __  \\ \\  \\\\ \\  \\  \n"
           "  \\|____|\\  \\ \\  \\\\\\  \\ \\  \\\\ \\  \\ \\  \\\\\\  \\ \\  \\|\\  \\ \\  \\ \\  \\ \\  \\\\ \\  \\ \n"
           "    ____\\_\\  \\ \\_______\\ \\__\\\\ \\__\\ \\_______\\ \\_______\\ \\__\\ \\__\\ \\__\\\\ \\__\\\n"
           "   |\\_________\\|_______|\\|__| \\|__|\\|_______|\\|_______|\\|__|\\|__|\\|__| \\|__|\n"
           "   \\|_________|\n\n\n");*/
    econio_textcolor(COL_LIGHTBLUE);
    printf("▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n");
    econio_textcolor(COL_CYAN);
    printf("       ███████  ██████  ██   ██  ██████  ██████   █████  ███    ██ \n"
           "       ██      ██    ██ ██  ██  ██    ██ ██   ██ ██   ██ ████   ██ \n"
           "       ███████ ██    ██ █████   ██    ██ ██████  ███████ ██ ██  ██ \n"
           "            ██ ██    ██ ██  ██  ██    ██ ██   ██ ██   ██ ██  ██ ██ \n"
           "       ███████  ██████  ██   ██  ██████  ██████  ██   ██ ██   ████ \n");
    econio_textcolor(COL_LIGHTBLUE);
    printf("▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n");

    econio_textcolor(COL_LIGHTCYAN);
    econio_textbackground(COL_RESET);
    econio_gotoxy(12,8);
    printf("NEHÉZSÉG: ");

    econio_gotoxy(16,20); printf("↑");
    econio_gotoxy(3,21); printf("Navigálás: ← ↓ →  ↲");

    int key = -1;
    enum State { exit, choseDificulty, choseLevel, startLvl};
    enum State state = choseDificulty;

    econio_rawmode();
    // Nehézség kiválasztása
    while(state != startLvl){
        switch (key){
            case KEY_ESCAPE:
                state--;
                break;
            case KEY_ENTER:
                state++;
                break;
            case KEY_RIGHT:
                if (state == choseDificulty && *selectedDificulty < maxDificulty) (*selectedDificulty)++;
                break;
            case KEY_LEFT:
                if (state == choseDificulty && *selectedDificulty > minDificulty) (*selectedDificulty)--;
                break;
            case KEY_UP:

                break;

            case KEY_DOWN:

                break;
        }
        // Display Dificulty Selecter
        if (state == choseDificulty){
            econio_gotoxy(23,8);
            for (int i = 0; i < 16; i++){
                // Kiválasztott szint kiemelése
                if (*selectedDificulty == i+1){
                    econio_textcolor(COL_BLUE);
                    econio_textbackground(COL_LIGHTCYAN);
                }
                else {
                    econio_textcolor(COL_LIGHTCYAN);
                    econio_textbackground(COL_RESET);
                }
                printf("%d", i+1);
                econio_textbackground(COL_RESET);
                printf(" ");
            }
        }


        key = econio_getch();
    }

    econio_gotoxy(12,12);
    printf("SZINT: ");
    key = -1;

    char hex[16] = "0123456789ABCDEF";
    // Szintek beolvasása
    DIR *d = opendir("./levels/");  // melyik mappa
    if (d == NULL) { perror("Nem sikerült megnyitni"); }
    else {
        struct dirent *de;
        while ((de = readdir(d)) != NULL) {
            if (de->d_name[0] == hex[*selectedDificulty]){
                printf("%s\n", de->d_name);
            }
        }
        closedir(d);
    }

//    econio_gotoxy(12,10);
//    printf("Selected: %d", *selectedDificulty);

    // Szint kiválasztása
//    while (key != KEY_ENTER){
//        if (key == KEY_DOWN && *selectedLevel < maxDificulty) (*selectedDificulty)++;
//        if (key == KEY_UP && *selectedLevel > minDificulty) (*selectedDificulty)--;
//    }


}

void PrintMap(char const **map, int width, int height){
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            printf("%c", map[y][x]);
        }
        printf("\n");
    }
}
void ReadXSBFile(char const filename[], char ***map){
//    char **newmap;
//    FILE* fp = fopen(filename, "r");
// foglalás
//    newmap = (char**) malloc(magassag * sizeof(char*));
//    for (int y = 0; y < magassag; ++y)
//        szamok[y] = (char*) malloc(szelesseg * sizeof(char));

}
/*int ToIndex(pos p){
    return p.x + p.y * totalWidth;
}
pos ToPos(int index){
    return (pos){index % totalWidth, index / totalWidth};
}*/
