/*
Raylib space-ship-game
This is my first game in raylib based on toutorial: https://www.youtube.com/watch?v=UoAsDlUwjy0&t=5343s

by Michal Niedbala 

*/
#include <stdlib.h>
#include <stdio.h>

#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "settings.h"
#include "timer.h"
#include "objects.h"


int main ()
{
	// Create the window and OpenGL context
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Ship Game");
	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	float dt;
	ObjectList* objList = (ObjectList*) malloc(sizeof(ObjectList));
	InitObjectList(objList);
	InitPlayer(objList, (Vector2) {SCREEN_WIDTH/2, SCREEN_HEIGHT-200}, PLAYER_SPEED);

	// Prepare random numer genertor
	unsigned int seed;
	seed = arc4random();
	srand(seed);

	Timer asteroidTimer;
	initTimer(&asteroidTimer, ASTEROID_TIMER_DURATION, true, true, CreateAsteroid, objList);

	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		if (IsKeyPressed(KEY_H)) {
			ToogleHitboxObjects(objList);
		}
		dt = GetFrameTime();
		updateTimer(&asteroidTimer);
		UpdateObjects(objList, dt);
		BeginDrawing();
		DrawObjects(objList);
		ClearBackground(BACKGROUND_COLOR);
		EndDrawing();
	}
	// cleanup
	CleanObjects(objList);
	CloseWindow();
	return 0;
}
