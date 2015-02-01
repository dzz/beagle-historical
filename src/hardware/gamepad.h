#ifndef __HW_GAMEPAD__
#define __HW_GAMEPAD__

#include <SDL.H>

typedef struct {
    int index;
    SDL_GameController* controller;
    char available;

    double left_x;
    double left_y;

    double right_x;
    double right_y;

} hw_gamepad;

void initGamepad();
void dropGamepad();
void GamepadHandleEvent(SDL_Event* event);

#endif
