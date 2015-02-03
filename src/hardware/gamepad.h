#ifndef __HW_GAMEPAD__
#define __HW_GAMEPAD__

#include <SDL.H>

#define MAX_PADS 8

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
hw_gamepad* getGamepads();
void GamepadHandleEvent(SDL_Event* event);
hw_gamepad* getGamepad(int index);
int GamepadDequeueIsDirty();


#endif