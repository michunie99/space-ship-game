#include <stdbool.h>
#include "raylib.h"

#ifndef PLAYER_H
#define PLAYER_H


typedef struct {
	Vector2 pos;
	Texture texture;
	float rotation;
	float scale;
	Color tint;
	bool visible;
} Sprite;

typedef struct {
	Vector2 startPos;
	Vector2 currPos;
	Vector2 direction;
	float speed;
	Sprite sprite;
} Player;

void InitPlayer(Player* p, Vector2 startPos, const char* texturePath);
void UpdatePlayer(Player *p);
void ResetPlayer(Player* p);
void UnloadPlayer(Player* p);

void InitSprite(Sprite* s, Vector2 pos, const char* texturePath, float rotation, float scale, Color tint, bool visible);
void RegisterSprite(Sprite* s);
void DrawAllSprites();
#endif