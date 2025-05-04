#include "raylib.h"

#ifndef COLLISIONS_H
#define COLLISIONS_H


#define MAP_ROWS 11
#define MAP_COLS 20
#define BLOCK_SIZE 100

typedef char Axis;

extern const char levelMap[MAP_ROWS][MAP_COLS];
extern Rectangle blocks[MAP_ROWS*MAP_COLS];
extern int blockNum;

void resolveCollision(Rectangle *player, Vector2 direction, const Axis axis);
void initMap();
#endif