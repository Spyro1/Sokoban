#include <ctype.h>
#include <stdio.h>
#include "../headers/lib.h"


void lib_WarningWindow(const char* Message, Point p, bool *displayFirst, int option, EconioColor baseColor, EconioColor accentForeColor, EconioColor accentBgColor) {
    int windowWidth = 26;
    if (*displayFirst) {
        *displayFirst = false;
        int i = 0;
        lib_ClearScrBellow();
        printfc("╔════════════════════════╗", p.x-windowWidth/2,p.y+i++,baseColor);
        printfc("║                        ║", p.x-windowWidth/2,p.y+i++,baseColor);
        printfc("║", p.x-windowWidth/2,p.y+i,baseColor);
        printfc(Message, p.x- stringlenght(Message)/2, p.y+i,baseColor);
        printfc("║", p.x+windowWidth/2-1,p.y+i++,baseColor);
        printfc("║                        ║", p.x-windowWidth/2,p.y+i++,baseColor);
        printfc("║     NEM       IGEN     ║", p.x-windowWidth/2,p.y+i++,baseColor);
        printfc("╚════════════════════════╝", p.x-windowWidth/2,p.y+i,baseColor);
    } // Kilépő ablak kiírása
    if (option == 0) {
        printfbc("NEM", p.x-windowWidth/2+6,p.y+4, accentForeColor, accentBgColor);
        printfbc("IGEN", p.x-windowWidth/2+16,p.y+4, baseColor, COL_RESET);
    }
    else {
        printfbc("NEM", p.x-windowWidth/2+6,p.y+4, baseColor, COL_RESET);
        printfbc("IGEN", p.x-windowWidth/2+16,p.y+4, accentForeColor, accentBgColor);
    }
}
void lib_ClearScrBellow(){
    lib_ClearScreenSection(0, 8, 73, 23, COL_RESET);
}
void lib_ClearScrLines(int lines){
    lib_ClearScreenSection(0, 8, 8 + lines, 23, COL_RESET);
}
void lib_ClearScreenSection(int x1, int y1, int x2, int y2, EconioColor bgColor){
    econio_gotoxy(x1,y1);
    econio_textcolor(COL_RESET);
    econio_textbackground(bgColor);
    for (int i = 0; i < y2-y1; i++){
        for(int j = 0; j < x2-x1; j++){
            printf(" ");
        }
        econio_gotoxy(x1,y1+i);
    }
}
void lib_printError(const char* errormessage){
    printf(errormessage, 7, 7, COL_RED);
}
void print(char const str[], int x, int y){
    econio_textbackground(COL_RESET);
    econio_gotoxy(x,y);
    printf("%s", str);
}
void printfc(char const str[], int x, int y, EconioColor foreColor){
    econio_textbackground(COL_RESET);
    econio_gotoxy(x,y);
    econio_textcolor(foreColor);
    printf("%s", str);
}
void printfbc(char const str[], int x, int y, EconioColor foreColor, EconioColor bgColor){
    econio_textbackground(COL_RESET);
    econio_gotoxy(x,y);
    econio_textcolor(foreColor);
    econio_textbackground(bgColor);
    printf("%s", str);
    econio_textbackground(COL_RESET);
}

int stringlenght(const char *str) {
    int count = 0;
    while (*str) {
        count += (*str++ & 0xC0) != 0x80;
    }
    return count;
}

int stringlenghtMax(const char *str, int max){
    int count = 0;
    int bytes = 0;
    while (*str && count < max) {
        count += (*str++ & 0xC0) != 0x80;
        bytes++;
    }
    return bytes;
}
bool isBlankString(const char* str) {
    while (*str != '\0') {
        if (!isspace((unsigned char)*str)) {
            return false; // Ha nem üres karatert talál, akkor kilép
        }
        str++;
    }
    return true; // Ha csak üres karaktert talált, akkor üres a string
}
//char *cutAndFixString(const char *str){
//    char newstr[nameLenght*2+1];
//    int i = 0;
//    while(*str != '\0'){
//        if (*str != ' ')
//    }
//}