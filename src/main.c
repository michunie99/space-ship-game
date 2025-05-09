/*
Raylib space-ship-game
This is my first game in raylib based on toutorial: https://www.youtube.com/watch?v=UoAsDlUwjy0&t=5343s

by Michal Niedbala 

*/
#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "settings.h"
#include "game_state.h"

int main ()
{
	// Create the window and OpenGL context
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Ship Game");
	InitAudioDevice();
	SearchAndSetResourceDir("resources");
	Game game;
	InitGame(&game);
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		UpdateGame(&game);
	}
	// cleanup
	ClearGame(&game);
	CloseWindow();
	return 0;
}
