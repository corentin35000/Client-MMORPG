#ifndef COMMON_H_
#define COMMON_H_

#include <cmath>
#include <vector>
// Données des maps
#define MAP_WIDTH 170
#define MAP_HEIGHT 255 
#define TILE_WIDTH 48
#define TILE_HEIGHT 32
#define NOMBRE_COLONNE MAP_WIDTH * TILE_WIDTH
#define NOMBRE_LIGNE MAP_HEIGHT * TILE_HEIGHT


enum keys { 
    UP,
    DOWN, 
    LEFT, 
    RIGHT,
    SPACE,
    MAX_KEY,
    FOO1,
    E
};

struct Point {
    int x, y;
    Point(int xx = 0, int yy = 0) : x(xx), y(yy) {};
};

struct Rect {
    float polygon[8];
};

extern int keyPressed;

struct Camera {
    int x, y;
};

extern Point tileCenter(int r, int c);

extern Point getTile(int x, int y);

extern Rect getTileRect(int r, int c, Camera camera);

#endif
