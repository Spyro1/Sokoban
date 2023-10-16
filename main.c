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
          colorBox = COL_BROWN,
          colorWall = COL_LIGHTRED,
          colorTarget = COL_GREEN,
          maxDificulty = 16,
          minDificulty = 1;


/* == Változók == */

int totalWidth = 25,  // Szélesség
    totalHeight = 25; // Magasság
pos playerPos; // A Játékos poziciója


// Függvények
void PrintMainScreen(int *selectedDificulty, int *selectedLevel);
void PrintMap(char map[]);
void ReadXSBFile(char filename[]);
int ToIndex(pos p);
pos ToPos(int index);


int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif
    // Főképernyő kiiratása és Szint választás
    int selectedDificulty = 1, selectedLevel = 1;
    PrintMainScreen(&selectedDificulty, &selectedLevel);

    char *map = (char*)malloc(totalHeight * totalWidth * sizeof(char)); // Ez lesz a dinamikus tömb ami a pályát tárolja
    ReadXSBFile(map);
    return 0;
}
void PrintMainScreen(int *selectedDificulty, int *selectedLevel){
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

    econio_rawmode();
    // Szint kiválasztása
    while(key != KEY_ENTER){
        if (key == KEY_RIGHT && *selectedDificulty < maxDificulty) (*selectedDificulty)++;
        if (key == KEY_LEFT && *selectedDificulty > minDificulty) (*selectedDificulty)--;
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
        key = econio_getch();
    }
//    econio_gotoxy(12,10);
//    printf("Selected: %d", *selectedDificulty);

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
}

void PrintMap(char *map){
    for(int i = 0; i < totalWidth*totalHeight; i++)
    {
        if (i % totalWidth == 0) printf("\n");
        printf("%c", map[i]);
    }
}
void ReadXSBFile(char filename[]){

}
int ToIndex(pos p){
    return p.x + p.y * totalWidth;
}
pos ToPos(int index){
    return (pos){index % totalWidth, index / totalWidth};
}
