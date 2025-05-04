/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/
#include <stdlib.h>
#include <stdio.h>

#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
// #include "objects.h"
// #include "collisions.h"
#include "timer.h"
#include "animation.h"



int main ()
{
	// Create the window and OpenGL context
	InitWindow(1920, 1080, "Collisions");
	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");
	Animation animation;
	InitAnimation(&animation, "animation", 0.5f);

	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		UpdateAnimationFrame(&animation, GetFrameTime());
		BeginDrawing();
		DrawFrameAnimation(&animation, (Vector2){10, 10}, 0, 1, WHITE);
		ClearBackground(BLACK);
		EndDrawing();
	}
	// cleanup
	CleanAnimation(&animation);
	CloseWindow();
	return 0;
}
