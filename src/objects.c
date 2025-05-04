#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#include "objects.h"



void InitSprite(Sprite* s, Vector2 pos, const char* texturePath, 
	float rotation, float scale, Color tint, bool visible) {
	s->pos = pos;
	s->texture = LoadTexture(texturePath);
	s->rotation = rotation;
	s->scale = scale;
	s->tint = tint;
	s->visible = visible;
}

void DrawSprite(Sprite* s) {
	if (!s->visible) return;

	DrawTextureEx(
		s->texture, 
		s->pos,
		s->rotation,
		s->scale, 
		s->tint
	);
}

// Global sprite holder
#define MAX_SPRITES 100
Sprite* spriteList[MAX_SPRITES];
int spriteCount = 0;

void RegisterSprite(Sprite* s) {
	if (spriteCount < MAX_SPRITES) {
		spriteList[spriteCount++] = s; 
	}
}

void DrawAllSprites() {
	for (int i = 0; i < spriteCount; i++) {
		DrawSprite(spriteList[i]);
	}
}

void UnloadSprite(Sprite* s) {
	UnloadTexture(s->texture);
}


void InitPlayer(Player* p, Vector2 startPos, const char* texturePath) {
	p->startPos = startPos;
	p->currPos = startPos;
	p->direction = (Vector2){0, 0};
	p->speed = 400;
	InitSprite(&p->sprite, p->currPos, texturePath, 0.0, 1.0, WHITE, true);
}

void UpdatePlayer(Player *p) {
	float dt = GetFrameTime();
	p->direction.x =  (int) IsKeyDown(KEY_D) - (int) IsKeyDown(KEY_A);
	p->direction.y =  (int) IsKeyDown(KEY_S) - (int) IsKeyDown(KEY_W);
	p->direction = Vector2Normalize(p->direction);
	p->currPos = Vector2Add(p->currPos, Vector2Scale(p->direction, dt*p->speed));	
	p->sprite.pos = p->currPos;
}

void ResetPlayer(Player* p) {
	p->currPos = p->startPos;
	p->direction = (Vector2){0, 0};
	p->speed = 100;
}

void UnloadPlayer(Player* p) {
	UnloadSprite(&p->sprite);
	free(p);
}