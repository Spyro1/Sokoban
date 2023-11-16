#include "../libraries/debugmalloc.h"
#include "../headers/menu.h"

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
    menu_MainScreen();
    // Debugmalloc loggolása fájlba
    debugmalloc_log_file("debugmalloc.txt");
    return 0;
}