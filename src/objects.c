#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#include "objects.h"
#include "settings.h"
#include <stdio.h>


void InitObjectList(ObjectList* objList) {
	objList->head = NULL;
	objList->tail = NULL;
	objList->count = 0;
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
	Object* obj = objList->head;
	while (obj) {
		obj->Draw(obj);
		obj = obj->next;		
	}
}

void UpdateObjects(ObjectList* objList, float dt) {
	Object* obj = objList->head;
	while (obj) {
		Object* next = obj->next;
		obj->Update(objList, obj, dt);
		obj = next;
	}
}

void CleanObjects(ObjectList* objList) {
	Object* obj = objList->head;
	while (obj) {
		obj->Clean(obj);
		obj = obj->next;		
	}
}


// void InitSprite(SpriteList* sl, Sprite* s, Vector2 pos, const char* texturePath, 
// 	float rotation, float scale, Color tint, bool visible) {
// 	s->pos = pos;
// 	s->texture = LoadTexture(texturePath);
// 	s->rotation = rotation;
// 	s->scale = scale;
// 	s->tint = tint;
// 	s->visible = visible;
// 	RegisterSprite(sl, s);
// }

void UpdatePlayer(ObjectList* objList, Object* player, float dt) {
	if (IsKeyPressed(KEY_SPACE)) {
		Vector2 laserPos = player->position;
		laserPos = Vector2Add(
			laserPos, 
			(Vector2){player->texture.width/2-5, -50}
		);
		InitLaser(
			objList,
			laserPos
		);
	}
	player->direction.x =  (int) IsKeyDown(KEY_D) - (int) IsKeyDown(KEY_A);
	player->direction.y =  (int) IsKeyDown(KEY_S) - (int) IsKeyDown(KEY_W);
	player->direction = Vector2Normalize(player->direction);
	player->position = Vector2Add(player->position, Vector2Scale(player->direction, dt*player->speed));	
}

void DrawPlayer(Object* player) {
	DrawTextureEx(
		player->texture,
		player->position,
		player->rotation,
		player->scale,
		WHITE
	);
}

void CleanPlayer(Object* player) {
	UnloadTexture(player->texture);
	free(player);
}

void UpdateLaser(ObjectList* objList, Object* laser, float dt) {
	laser->direction = Vector2Normalize(laser->direction);
	laser->position = Vector2Add(laser->position, Vector2Scale(laser->direction, dt*laser->speed));	

	if (laser->position.x < -laser->texture.width || 
		laser->position.x > SCREEN_WIDTH+laser->texture.width || 
		laser->position.y < -laser->texture.height
	) {
		UnregisterObject(objList, laser);
	}
}

void DrawLaser(Object* laser) {
	DrawTextureEx(
		laser->texture,
		laser->position,
		laser->rotation,
		laser->scale,
		WHITE
	);
}

void CleanLaser(Object* laser) {
	UnloadTexture(laser->texture);
	free(laser);
}

// void InitObject(
// 	Object* obj, 
// 	ObjectType type, 
// 	Vector2 startPos, 
// 	Vector2 direction, 
// 	float speed,
// 	bool active
// ) {
// 	switch (type) {
// 		case PLAYER_TYPE:
// 		obj->texture = LoadTexture("images/spaceship.png");
// 		obj->rotation = 0.0f;
// 		obj->scale = 1.0f;
// 		obj->tint = WHITE;
// 		obj->Update = UpdatePlayer;
// 		obj->Draw = DrawPlayer;
// 		obj->Clean = CleanPlayer;
// 		break;
// 		case LASER_TYPE:
// 		obj->texture = LoadTexture("images/laser.png");
// 		obj->rotation = 0.0f;
// 		obj->scale = 1.0f;
// 		obj->tint = WHITE;
// 		obj->Update = UpdateLaser;
// 		obj->Draw = DrawLaser;
// 		obj->Clean = CleanLaser;
// 		break;
// 		case ASTEROID_TYPE:
// 		break;
// 		default:
// 	}
// 	obj->visible = true;

// 	obj->type = type;
// 	obj->position = startPos;
// 	obj->direction = direction;
// 	obj->speed = speed;
// 	obj->active = active;
// }


void InitPlayer(ObjectList* objList, Vector2 startPos, float speed) {
	Object* obj = (Object*) malloc(sizeof(Object));
	*obj = (Object) {
		.type = PLAYER_TYPE,
		.position = startPos,
		.direction = (Vector2){0, 0},
		.speed = speed,
		.active = true,
		.texture = LoadTexture("images/spaceship.png"),
		.rotation = 0.0,
		.scale = 1.0,
		.tint = WHITE,
		.visible = true,
		.next = NULL,
		.prev = NULL,
		.Update = UpdatePlayer,
		.Draw = DrawPlayer,
		.Clean = CleanPlayer
	};
	RegisterObject(objList, obj);	
}

void InitLaser(ObjectList* objList, Vector2 startPos) {
	Object* obj = (Object*) malloc(sizeof(Object));
	*obj = (Object) {
		.type = LASER_TYPE,
		.position = startPos,
		.direction = (Vector2){0, -1},
		.speed = LASER_SPEED,
		.active = true,
		.texture = LoadTexture("images/laser.png"),
		.rotation = 0.0,
		.scale = 1.0,
		.tint = WHITE,
		.visible = true,
		.next = NULL,
		.prev = NULL,
		.Update = UpdateLaser,
		.Draw = DrawLaser,
		.Clean = CleanLaser
	};
	RegisterObject(objList, obj);	
}

// void InitLaser(SpriteList* sl, Laser* l, Vector2 startPos, float speed) {
// 	l->startPos = startPos;
// 	l->currPos = startPos;
// 	l->direction = (Vector2){0, -1};
// 	l->speed = speed;
// 	InitSprite(sl, &l->sprite, l->currPos,  "images/laser.png", 0.0, 1.0, WHITE, true);
// }

// void UpdateLasers(Laser *l, float dt) {
// 	l->direction.x =  (int) IsKeyDown(KEY_D) - (int) IsKeyDown(KEY_A);
// 	l->direction.y =  (int) IsKeyDown(KEY_S) - (int) IsKeyDown(KEY_W);
// 	l->direction = Vector2Normalize(l->direction);
// 	l->currPos = Vector2Add(l->currPos, Vector2Scale(l->direction, dt*l->speed));	
// 	l->sprite.pos = l->currPos;
// }


// void CleanLaser(SpriteList* sl , Laser* l) {
// 	UnregisterSprite(sl, &l->sprite);
// 	free(l);
// }