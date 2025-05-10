#include "animation.h"
#include "stdlib.h"

void InitAnimation(Animation* animation, Texture2D* textures, int numberOfFrames, const float delay, bool repeat) {
	animation->textures = textures;
	animation->currentFrame=0;
	animation->numberOfFrames=numberOfFrames;
	animation->frameTime=0;
	animation->delay=delay;
	animation->repeat=repeat;
	animation->finished=false;
}

void UpdateAnimationFrame(Animation* animation, const float dt) {
	if (animation->finished) {return;}
	animation->frameTime+=dt;
	if (animation->frameTime >= animation->delay) {
		animation->currentFrame++;
		if (
			!animation->repeat && 
			animation->currentFrame >= animation->numberOfFrames
		) {
			animation->finished=true;
		} else {
			animation->currentFrame=animation->currentFrame % animation->numberOfFrames;
		}
		animation->frameTime = 0;
	}
}

void DrawFrameAnimation(Animation* animation, Vector2 pos, float rotation, float scale, Color tint) {
	DrawTextureEx(animation->textures[animation->currentFrame], pos, rotation, scale, tint);
}

void CleanAnimation(Animation* animation) {
	animation->textures = NULL;
	animation->currentFrame=0;
	animation->numberOfFrames=0;
	free(animation);
}