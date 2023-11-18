#include "../libraries/econio.h"
#include "../headers/datatypes.h"

Point addPoints(Point a, Point b){
    return (Point) {a.x + b.x, a.y + b.y};
}
Point subPoints(Point a, Point b){
    return (Point) {a.x - b.x, a.y - b.y};
}
bool comparePoints(Point a, Point b){
    return  a.x == b.x && a.y == b.y;
}

