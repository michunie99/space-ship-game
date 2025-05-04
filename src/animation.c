#include "animation.h"
#include "stdlib.h"

void InitAnimation(Animation* animation, const char* animationPath, const float delay) {
	if (!DirectoryExists(animationPath)) {
		exit(EXIT_FAILURE);
	}
	FilePathList frames = LoadDirectoryFiles(animationPath);
	animation->textures = (Texture2D*) malloc(frames.count*sizeof(Texture2D));
	if (animation->textures == NULL) {
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < frames.count; i++) {
		animation->textures[i] = LoadTexture(frames.paths[i]);
	}
	animation->currentFrame=0;
	animation->numberOfFrames=frames.count;
	animation->frameTime=0;
	animation->delay=delay;
}

void UpdateAnimationFrame(Animation* animation, const float dt) {
	animation->frameTime+=dt;
	if (animation->frameTime >= animation->delay) {
		animation->currentFrame++;
		animation->currentFrame=animation->currentFrame % animation->numberOfFrames;
		animation->frameTime = 0;
	}
}

void DrawFrameAnimation(Animation* animation, Vector2 pos, float rotation, float scale, Color tint) {
	DrawTextureEx(animation->textures[animation->currentFrame], pos, rotation, scale, tint);
}

void CleanAnimation(Animation* animation) {
	for (int i = 0; i < animation->numberOfFrames; i++) {
		UnloadTexture(animation->textures[i]);
	}
	free(animation->textures);
	animation->textures = NULL;
	animation->currentFrame=0;
	animation->numberOfFrames=0;
}