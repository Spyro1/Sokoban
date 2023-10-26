#include "debugmalloc.h"
#include "datatypes.h"

Point AddPoints(Point a, Point b){
    return (Point) {a.x + b.x, a.y + b.y};
}
Point SubPoints(Point a, Point b){
    return (Point) {a.x - b.x, a.y - b.y};
}
bool EqualToPoint(Point a, Point b){
    return  a.x == b.x && a.y == b.y;
}