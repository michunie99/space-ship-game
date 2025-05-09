#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "settings.h"
#include "timer.h"
#include "objects.h"
#include "collisions.h"
#include <stdio.h>

#ifndef GAME_STATE_H
#define GAME_STATE_H

#define MAX_TIMER_NUMBER 10

typedef enum {
    NOT_INITIALIZED = 0,
    ACTIVE,
    PAUSED,
    FINISHED

} GameStatus;

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
} Game;


static GameStatus globalGame = NOT_INITIALIZED;

void ResetGame(Game* game) {
    game->gs=ACTIVE;
    CleanObjects(game->objList);
	InitObjectList(game->objList);
	InitPlayer(game->objList, (Vector2) {SCREEN_WIDTH/2, SCREEN_HEIGHT-200}, PLAYER_SPEED);
	game->player = game->objList->head;
	game->hitBoxesVisible = false;
}

void ClearGame(Game* game) {
    UnloadMusicStream(game->soundTrack);
	CleanObjects(game->objList);
}

void InitGame (Game* game) {
    game->gs=ACTIVE;
	game->font = LoadFont("font/Stormfaze.otf");
    game->soundTrack = LoadMusicStream("audio/music.wav");
    PlayMusicStream(game->soundTrack);
    game->objList = (ObjectList*) malloc(sizeof(ObjectList));
	InitObjectList(game->objList);
	InitPlayer(game->objList, (Vector2) {SCREEN_WIDTH/2, SCREEN_HEIGHT-200}, PLAYER_SPEED);
	game->player = game->objList->head;
	game->seed = arc4random();
	srand(game->seed);
	game->hitBoxesVisible = false;
	initTimer(&game->timers[0], ASTEROID_TIMER_DURATION, true, true, CreateAsteroid, game->objList);
}

void UpdateGame(Game* game) {
    game->dt = GetFrameTime();
    UpdateMusicStream(game->soundTrack);
    switch (game->gs)
    {
    case ACTIVE:
        if (IsKeyPressed(KEY_P)) {
            game->gs = PAUSED;
            PauseMusicStream(game->soundTrack);
        }
		if (IsKeyPressed(KEY_H)) {
			if (game->hitBoxesVisible) {
				HideHitboxObjects(game->objList);
			} else {
				ShowHitboxObjects(game->objList);
			}
			game->hitBoxesVisible = !game->hitBoxesVisible;

		}
        if (IsKeyPressed(KEY_F)) {
            game->drawFps = !game->drawFps;
        }
		updateTimer(&game->timers[0]);
		UpdateObjects(game->objList, game->dt);
		resolveCollision(game->objList);
        if (game->player->destroied) {
            game->gs = FINISHED;
            PauseMusicStream(game->soundTrack);
        }

		BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);
        if (game->drawFps) {DrawFPS(0, 0);}
        DrawObjects(game->objList);

        char score[100];
        snprintf(score, 100, "%d", game->objList->score);
        DrawTextEx(game->font, score, (Vector2){SCREEN_WIDTH/2-(4*80), SCREEN_HEIGHT/2}, 80, 20, WHITE);
		EndDrawing();
        break;

    case PAUSED:
        if (IsKeyPressed(KEY_P)) {
            game->gs = ACTIVE;
            ResumeMusicStream(game->soundTrack);
        }
		BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);
        DrawObjects(game->objList);
        DrawTextEx(game->font, "PAUSED", (Vector2){SCREEN_WIDTH/2-(4*80), SCREEN_HEIGHT/2}, 80, 20, WHITE);
		EndDrawing();
        break;

    case FINISHED:
        if (IsKeyPressed(KEY_R)) {
            ResetGame(game);
            SeekMusicStream(game->soundTrack, 0.0);
            ResumeMusicStream(game->soundTrack);

        }
		BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);
        DrawTextEx(game->font, "GAME OVER", (Vector2){SCREEN_WIDTH/2-(4*80), SCREEN_HEIGHT/2}, 80, 20, WHITE);
		EndDrawing();
        break;

    }
}

#endif