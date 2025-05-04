#include "raylib.h"

#ifndef ANIMATION_H
#define ANIMATION_H


typedef struct {
	Texture2D* textures;
	int currentFrame;
	int numberOfFrames;
	float frameTime;
	float delay;
} Animation;

void InitAnimation(Animation* animation, const char* animationPath, const float delay);
void UpdateAnimationFrame(Animation* animation, const float dt);
void DrawFrameAnimation(Animation* animation, Vector2 pos, float rotation, float scale, Color tint);
void CleanAnimation(Animation* animation);

#endif