#include "debugmalloc.h"
#include "datatypes.h"
#include "econio.h"

Point AddPoints(Point a, Point b){
    return (Point) {a.x + b.x, a.y + b.y};
}
Point SubPoints(Point a, Point b){
    return (Point) {a.x - b.x, a.y - b.y};
}
bool EqualToPoint(Point a, Point b){
    return  a.x == b.x && a.y == b.y;
}

void ClearScrBellow(){
    ClearScreenSection(0, 8, 73, 22, COL_RESET);
}
void ClearScreenSection(int x1, int y1, int x2, int y2, EconioColor bgColor){
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
//int StringLenght(const char *str){
//    int lenght = 0;
//    while (str[lenght] != '\0'){
//        switch (str) {
//            case "á":
//            case "é":
//                lenght +=2;
//                break;
//            default:
//                lenght++;
//                break;
//        }
//    }
//}
