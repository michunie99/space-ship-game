#include "raylib.h"

#ifndef SETTINGS_H
#define SETTINGS_H

static const int SCREEN_WIDTH = 1920/2;
static const int SCREEN_HEIGHT = 1080/2;

static const int PLAYER_SPEED = 500;
static const int LASER_SPEED = 600;
static const int METEOR_SPEED_RANGE[2] = {300, 400};
static const float METEOR_TIMER_DURATION = 0.4;

static const Color BACKGROUND_COLOR = GRAY;

#endif