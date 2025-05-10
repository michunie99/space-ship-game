#include "raylib.h"
#include "settings.h"
#include "timer.h"
#include "objects.h"

#include <stdio.h>

#ifndef GAME_STATE_H
#define GAME_STATE_H

#define MAX_TIMER_NUMBER 10
#define STARS_NUMBER 30


typedef enum {
    NOT_INITIALIZED = 0,
    ACTIVE,
    PAUSED,
    FINISHED

} GameStatus;


typedef struct {
    Vector2 position;
    float rotation;
    float scale;
} Star;

typedef struct {
    GameStatus gs;
    ObjectList* objList;
    Timer timers[MAX_TIMER_NUMBER];
    Font font;
    Object* player;
    Music soundTrack;

    float dt;
    unsigned int seed;
    bool hitBoxesVisible;
    bool drawFps;

    Texture2D starTexture;
    Star stars[STARS_NUMBER];
} Game;


static GameStatus globalGame = NOT_INITIALIZED;

void ResetGame(Game* game);
void ClearGame(Game* game);
void InitGame (Game* game);
void UpdateGame(Game* game);

#endif