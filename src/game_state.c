#include "game_state.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "collisions.h"
#include "gui.h"

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _b : _a; })

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
	game->font = LoadFontEx("font/Stormfaze.otf", FONT_SIZE, NULL, 0);
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

    game->starTexture = LoadTexture("images/star.png");
    for (int i=0; i<STARS_NUMBER; i++) {
        game->stars[i] = (Star) {
            .position = (Vector2) {
                ((float) rand() / RAND_MAX) * SCREEN_WIDTH,
                ((float) rand() / RAND_MAX) * SCREEN_HEIGHT
            },
            .rotation = ((float) rand() / RAND_MAX) * PI,
            .scale = max(0.5, ((float) rand() / RAND_MAX))
        };
    }
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
        DrawStars(game);
        if (game->drawFps) {DrawFPS(0, 0);}
        DrawObjects(game->objList);
        DrawScore(game);
		EndDrawing();
        break;

    case PAUSED:
        if (IsKeyPressed(KEY_P)) {
            game->gs = ACTIVE;
            ResumeMusicStream(game->soundTrack);
        }
		BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);
        DrawStars(game);
        DrawPaused(game);
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
        // Add Draw
        DrawGameOver(game);
		EndDrawing();
        break;

    }
}