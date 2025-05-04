#include <stdbool.h>
#include "raylib.h"

#ifndef PLAYER_H
#define PLAYER_H

#define MAX_LASERS 100
#define MAX_ASTEROIDS 100

typedef enum {
	PLAYER_TYPE,
	LASER_TYPE,
	ASTEROID_TYPE
} ObjectType;

typedef struct ObjectList ObjectList;

typedef struct Object {
    ObjectType type;
    Vector2 position;
    Vector2 direction;
    float speed;
    bool active;

    Texture2D texture;
    float rotation;
    float scale;
    Color tint;
    bool visible;

    struct Object* next;
    struct Object* prev;

    void (*Update)(ObjectList* objList, struct Object* self, float dt);
    void (*Draw)(struct Object* self);
    void (*Clean)(struct Object* self);
	// Add validate function
} Object;


struct ObjectList {
	Object* head;
	Object* tail;
	int count;
};

void InitObjectList(ObjectList* objList);
void RegisterObject(ObjectList* objList, Object* obj);
void UnregisterObject(ObjectList* objList, Object* obj);
void DrawObjects(ObjectList* objList);
void UpdateObjects(ObjectList* objList, float dt);
void CleanObjects(ObjectList* objList);

void InitPlayer(ObjectList* objList, Vector2 startPos, float speed);
void InitLaser(ObjectList* objList, Vector2 startPos);
void InitAsteroid(ObjectList* objList);

#endif