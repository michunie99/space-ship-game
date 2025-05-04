#include "collisions.h"
#include <stdlib.h>

const char levelMap[MAP_ROWS][MAP_COLS] = {
    "1111111111111111111",
    "1010000000000000001",
    "1010000000001111111",
    "1000000000000000111",
    "1000000200000000011",
    "1000000000000100001",
    "1000000000000100001",
    "1001100000000100001",
    "1001100000000100001",
    "1001100000000100001",
    "1111111111111111111"	
};
Rectangle blocks[MAP_ROWS*MAP_COLS];
int blockNum = 0;

void resolveCollision(Rectangle *player, Vector2 direction, const Axis axis) {
	for (int i=0; i<MAP_ROWS*MAP_COLS; i++) {
		if (CheckCollisionRecs(*player, blocks[i])) {
			switch (axis) {
				case 'x':
					if (direction.x > 0) {player->x = blocks[i].x - player->width;} else if (direction.x < 0) {player->x = blocks[i].x + blocks[i].width;}
					break;
				case 'y':
					if (direction.y > 0) {player->y = blocks[i].y - player->height;} else if (direction.y < 0) {player->y = blocks[i].y + blocks[i].height;}
					break;
				default:
					exit(EXIT_FAILURE);
			}
		}
	}
}

void initMap() {
	for (int row=0; row<MAP_ROWS; row++){
		for (int col=0; col<MAP_COLS; col++) {
			if (levelMap[row][col] == '1') {
				int x, y;
				x = col* BLOCK_SIZE;
				y = row * BLOCK_SIZE;
				blocks[blockNum++] = (Rectangle){x,y, BLOCK_SIZE, BLOCK_SIZE};
			}
		}
	}
}