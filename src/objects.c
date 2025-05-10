#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#include "objects.h"
#include "settings.h"

#include <stdlib.h>
#include <stdio.h>

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _b : _a; })

void InitObjectList(ObjectList* objList) {
	objList->head = NULL;
	objList->tail = NULL;
	objList->count = 0;
	objList->score = 0;

	objList->textures[ASTEROID_TYPE] = LoadTexture("images/meteor.png");
	objList->textures[PLAYER_TYPE] = LoadTexture("images/spaceship.png");
	objList->textures[LASER_TYPE] = LoadTexture("images/laser.png");

	objList->sounds[LASER_TYPE] = LoadSound("audio/laser.wav");
	objList->sounds[ASTEROID_TYPE] = LoadSound("audio/explosion.wav");

	FilePathList frames = LoadDirectoryFiles("images/explosion/");
	objList->animationSize = frames.count;
	for (int i = 0; i < frames.count; i++) {
		objList->animation[i] = LoadTexture(frames.paths[i]);
	}
	
}

void RegisterObject(ObjectList* objList, Object* obj) {
	if (objList->head == NULL) {
		objList->head = obj;
		objList->tail = obj;
		obj->prev = NULL;
		obj->next = NULL;
	} else {
		obj->prev = objList->tail;
		objList->tail->next = obj;
		obj->next = NULL;
		objList->tail = obj;
	}
	objList->count++;
}

void UnregisterObject(ObjectList* objList, Object* obj) {
	if (objList->head == obj) {
		objList->head = obj->next;
		obj->next->prev = NULL;		
	} else if (objList->tail == obj) {
		objList->tail = obj->prev;
		obj->prev->next = NULL;
	} else {
		Object* prev = obj->prev;
		Object* next = obj->next;
		if (prev) {
			prev->next=next;
		}
		if (next) {
			next->prev=prev;
		} 
	}
	obj->Clean(obj);
	objList->count--;
}

void DrawObjects(ObjectList* objList) {
	Object* obj = objList->tail;
	while (obj) {
		if (obj->visible) {
			obj->Draw(obj);
		}
		obj = obj->prev;		
	}
}

void UpdateObjects(ObjectList* objList, float dt) {
	Object* obj = objList->head;
	while (obj) {
		Object* next = obj->next;
		if (obj->active) {
			obj->Update(objList, obj, dt);
		}
		obj = next;
	}
}

void CleanObjects(ObjectList* objList) {
	Object* obj = objList->head;
	while (obj) {
		obj->Clean(obj);
		obj = obj->next;		
	}
	UnloadTexture(objList->textures[LASER_TYPE]);
	UnloadTexture(objList->textures[ASTEROID_TYPE]);
	UnloadTexture(objList->textures[PLAYER_TYPE]);
	for (int i = 0; i < objList->animationSize; i++) {
		UnloadTexture(objList->animation[i]);
	}

	UnloadSound(objList->sounds[LASER_TYPE]);
	UnloadSound(objList->sounds[ASTEROID_TYPE]);
}

// TODO: change to disabl/exenable
void ShowHitboxObjects(ObjectList* objList) {
	Object* obj = objList->head;
	while (obj) {
		obj->hitboxVisible=true;
		obj = obj->next;		
	}
}

void HideHitboxObjects(ObjectList* objList) {
	Object* obj = objList->head;
	while (obj) {
		obj->hitboxVisible=false;
		obj = obj->next;		
	}
}


void UpdatePlayer(ObjectList* objList, Object* player, float dt) {
	if (IsKeyPressed(KEY_SPACE)) {
		Vector2 laserPos = player->position;
		laserPos = Vector2Add(
			laserPos, 
			(Vector2){player->texture->width/2-5, 0}
		);
		InitLaser(
			objList,
			laserPos
		);
		if (objList->tail) {
			objList->tail->hitboxVisible = player->hitboxVisible;
		}
		PlaySound(*objList->tail->sound);
	}
	player->direction.x =  (int) IsKeyDown(KEY_D) - (int) IsKeyDown(KEY_A);
	player->direction.y =  (int) IsKeyDown(KEY_S) - (int) IsKeyDown(KEY_W);
	player->direction = Vector2Normalize(player->direction);
	player->position = Vector2Add(player->position, Vector2Scale(player->direction, dt*player->speed));	
	player->position.x = min(max(0, player->position.x), SCREEN_WIDTH - player->texture->width);
	player->position.y = min(max(0, player->position.y), SCREEN_HEIGHT - player->texture->height);
	player->hitbox.x = player->position.x;
	player->hitbox.y = player->position.y;
}

void DrawPlayer(Object* player) {
	DrawTextureEx(
		*(player->texture),
		player->position,
		player->rotation,
		player->scale,
		WHITE
	);
	if (player->hitboxVisible) {
		DrawRectangleLinesEx(player->hitbox, 2, GREEN);
	}
}

void CleanPlayer(Object* player) {
	free(player);
}

void UpdateLaser(ObjectList* objList, Object* laser, float dt) {
	if (laser->destroied) {
		UnregisterObject(objList, laser);
		return;
	}
	laser->direction = Vector2Normalize(laser->direction);
	laser->position = Vector2Add(laser->position, Vector2Scale(laser->direction, dt*laser->speed));	
	laser->hitbox.x = laser->position.x;
	laser->hitbox.y = laser->position.y;

	if (laser->position.x < -laser->texture->width || 
		laser->position.x > SCREEN_WIDTH+laser->texture->width || 
		laser->position.y < -laser->texture->height
	) {
		laser->destroied=true;
	}
}

void DrawLaser(Object* laser) {
	DrawTextureEx(
		*laser->texture,
		laser->position,
		laser->rotation,
		laser->scale,
		WHITE
	);
	if (laser->hitboxVisible) {
		DrawRectangleLinesEx(laser->hitbox, 2, BLUE);
	}
}

void CleanLaser(Object* laser) {
	free(laser);
}

void UpdateAsteriod(ObjectList* objList, Object* asteroid, float dt) {
	if (asteroid->destroied){
		if (asteroid->animation->finished) {
			UnregisterObject(objList, asteroid);
			return;
		} else if (asteroid->animationActive) {
			UpdateAnimationFrame(asteroid->animation, dt);
			return;
		}
	}
	asteroid->direction = Vector2Normalize(asteroid->direction);
	asteroid->position = Vector2Add(asteroid->position, Vector2Scale(asteroid->direction, dt*asteroid->speed));
	asteroid->hitbox.x = asteroid->position.x;
	asteroid->hitbox.y = asteroid->position.y;

	if (asteroid->position.x < -asteroid->texture->width || 
		asteroid->position.x > SCREEN_WIDTH+asteroid->texture->width || 
		asteroid->position.y > SCREEN_HEIGHT
	) {
		asteroid->animation->finished=true;
		asteroid->destroied=true;
	}
}

void DrawAsteriod(Object* asteroid) {
	if (asteroid->animationActive) {
		DrawFrameAnimation(
			asteroid->animation,
			Vector2Add(asteroid->position, (Vector2){20, 20}),
			asteroid->rotation,
			asteroid->scale,
			WHITE
		);
	} else {
		DrawTextureEx(
			*asteroid->texture,
			asteroid->position,
			asteroid->rotation,
			asteroid->scale,
			WHITE
		);
	}
	if (asteroid->hitboxVisible) {
		DrawRectangleLinesEx(asteroid->hitbox, 2, RED);
	}
}

void CleanAsteriod(Object* asteroid) {
	CleanAnimation(asteroid->animation);
	free(asteroid);
}

void InitPlayer(ObjectList* objList, Vector2 startPos, float speed) {
	Object* obj = (Object*) malloc(sizeof(Object));
	Texture2D* texture = &objList->textures[PLAYER_TYPE];
	*obj = (Object) {
		.type = PLAYER_TYPE,
		.position = startPos,
		.direction = (Vector2){0, 0},
		.speed = speed,
		.active = true,
		.texture = texture,
		.rotation = 0.0,
		.scale = 1.0,
		.tint = WHITE,
		.visible = true,
		.next = NULL,
		.prev = NULL,
		.Update = UpdatePlayer,
		.Draw = DrawPlayer,
		.Clean = CleanPlayer,
		.hitbox = (Rectangle){startPos.x, startPos.y, texture->width, texture->height},
		.hitboxVisible = false,
		.destroied = false,
		.animation = NULL,
		.animationActive = false,
		.sound = NULL
	};
	RegisterObject(objList, obj);	
}

void InitLaser(ObjectList* objList, Vector2 startPos) {
	Object* obj = (Object*) malloc(sizeof(Object));
	Texture2D* texture = &objList->textures[LASER_TYPE];
	Sound* sound = &objList->sounds[LASER_TYPE];
	*obj = (Object) {
		.type = LASER_TYPE,
		.position = startPos,
		.direction = (Vector2){0, -1},
		.speed = LASER_SPEED,
		.active = true,
		.texture = texture,
		.rotation = 0.0,
		.scale = 1.0,
		.tint = WHITE,
		.visible = true,
		.next = NULL,
		.prev = NULL,
		.Update = UpdateLaser,
		.Draw = DrawLaser,
		.Clean = CleanLaser,
		.hitbox = (Rectangle){startPos.x, startPos.y, texture->width, texture->height},
		.hitboxVisible = false,
		.destroied = false,
		.animation = NULL,
		.animationActive = false,
		.sound = sound
	};
	RegisterObject(objList, obj);	
}

void InitAsteroid(ObjectList* objList) {
	Object* obj = (Object*) malloc(sizeof(Object));
	float speed = ASTEROID_SPEED_RANGE[0] + (ASTEROID_SPEED_RANGE[1] - ASTEROID_SPEED_RANGE[0]) * (float) rand() / RAND_MAX;
	Texture2D* texture = &objList->textures[ASTEROID_TYPE];
	Vector2 startPos = (Vector2) {(texture->width + rand()) % (SCREEN_WIDTH-texture->width), -texture->height};
	float angle = (((float) rand() / (RAND_MAX / 2)) - 1) * PI / 5;
	Vector2 direction = Vector2Rotate((Vector2) {0, 1}, angle);

	Animation* animation = malloc(sizeof(Animation));
	InitAnimation(animation, objList->animation, objList->animationSize, 0.01, false);
	Sound* sound = &objList->sounds[ASTEROID_TYPE];

	*obj = (Object) {
		.type = ASTEROID_TYPE,
		.position = startPos,
		.direction = direction,
		.speed = speed,
		.active = true,
		.texture = texture,
		.rotation = 0.0,
		.scale = 1.0,
		.tint = WHITE,
		.visible = true,
		.next = NULL,
		.prev = NULL,
		.Update = UpdateAsteriod,
		.Draw = DrawAsteriod,
		.Clean = CleanAsteriod,
		.hitbox = (Rectangle){startPos.x, startPos.y, texture->width, texture->height},
		.hitboxVisible = false,
		.destroied = false,
		.animation = animation,
		.animationActive = false,
		.sound = sound
	};
	RegisterObject(objList, obj);	
	
}

void CreateAsteroid(void* data) {
	ObjectList* objList = (ObjectList*) data;
	bool hitboxVisible = false;
	if (objList->head) {
		hitboxVisible = objList->head->hitboxVisible;
	}
	InitAsteroid(objList);
	objList->tail->hitboxVisible = hitboxVisible;
}