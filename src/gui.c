#include "gui.h"
#include "settings.h"
#include "raylib.h"

#include <stdio.h>

#define MAX_TEXT_LEN 100

void DrawScore(Game* game) {
    char text[MAX_TEXT_LEN];
    float fontSize = game->font.baseSize;
    float fontSpacing = 2;
    Vector2 size = { 0 };
    snprintf(text, MAX_TEXT_LEN, "%d",  game->objList->score);
    size = MeasureTextEx(game->font, text, fontSize, fontSpacing);
    DrawTextEx(game->font, text, (Vector2){SCREEN_WIDTH/2 - size.x/2, size.y}, fontSize, fontSpacing, WHITE);
}

void DrawGameOver(Game* game) {
    char text[MAX_TEXT_LEN];
    float fontSize = game->font.baseSize;
    float fontSpacing = 2;
    Vector2 size = { 0 };
    snprintf(text, MAX_TEXT_LEN, "GAME OVER");
    size = MeasureTextEx(game->font, text, fontSize, fontSpacing);
    DrawTextEx(game->font, text, (Vector2){SCREEN_WIDTH/2 - size.x/2, SCREEN_HEIGHT/3}, fontSize, fontSpacing, WHITE);

    snprintf(text, MAX_TEXT_LEN, "Press [r] to restart");
    float p_height = size.y;
    size = MeasureTextEx(game->font, text, fontSize*0.5, fontSpacing);
    DrawTextEx(game->font, text, (Vector2){SCREEN_WIDTH/2 - size.x/2, SCREEN_HEIGHT/3 + p_height*1.2}, fontSize*0.5, fontSpacing, WHITE);
}

void DrawStars(Game* game) {
    for (int i=0; i<STARS_NUMBER; i++) {
        DrawTextureEx(
            game->starTexture, 
            game->stars[i].position,
            game->stars[i].rotation,
            game->stars[i].scale,
            WHITE
        );
    }
}


void DrawPaused(Game* game) {
    char text[MAX_TEXT_LEN];
    float fontSize = game->font.baseSize;
    float fontSpacing = 2;
    Vector2 size = { 0 };
    snprintf(text, MAX_TEXT_LEN, "PAUSED");
    size = MeasureTextEx(game->font, text, fontSize, fontSpacing);
    DrawTextEx(game->font, text, (Vector2){SCREEN_WIDTH/2 - size.x/2, SCREEN_HEIGHT/3}, fontSize, fontSpacing, WHITE);

    snprintf(text, MAX_TEXT_LEN, "Press [p] to resume");
    float p_height = size.y;
    size = MeasureTextEx(game->font, text, fontSize*0.5, fontSpacing);
    DrawTextEx(game->font, text, (Vector2){SCREEN_WIDTH/2 - size.x/2, SCREEN_HEIGHT/3 + p_height*1.2}, fontSize*0.5, fontSpacing, WHITE);
}