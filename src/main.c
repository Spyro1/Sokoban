#include "../headers/menu.h"
#include "../libraries/debugmalloc.h"

#ifdef _WIN32
    #include <windows.h>
#endif

int main() {
    // Karakter kódolás beállítása
    #ifdef _WIN32
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
    #endif

    // Főmenü meghívása
    // Csak a játék indításához kommenteld a következő sort
     MainScreen();

    // == TESZT == : Csak a játék indításához uncoment-eld ezt a részt TESZT END kommentig
        /*// Változók deklarálása
        int numOfLevels; // A beolvasott szintek száma a ./levels/ mappából, levelList tömb hossza
        Player currentPlayer; // A menüben kiválasztott játékost tartalmazza
        char **levelList; // A szintek fájlneveit tartalmazza a ./levels/ mappából, dinamikus string tömb
        // Szintek beolvasása a ./levels/ mappából
        ReadDirectoryLevelNames("./levels/", &levelList, &numOfLevels);
        currentPlayer = (Player) {"Marci", 0};
        // Játék inicializálása
        Init(&currentPlayer, levelList);
        FreeLevelList(&levelList, &numOfLevels);*/
    // == TESZT END ==

    // Debugmalloc loggolása fájlba
    debugmalloc_log_file("debugmalloc.txt");
    return 0;
}

