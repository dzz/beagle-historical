#include <SDL.H>

#include "gamepad.h"
#include <stdio.h>

hw_gamepad Gamepads[MAX_PADS];
hw_gamepad* MappedPads[MAX_PADS];

int _dirty = 0;

int GamepadDequeueIsDirty() {
    if(_dirty=1) {
        _dirty=0;
        return 1;
    }
    return 0;
}

void resetGamepad() {
    int i;

    for(i=0; i<MAX_PADS; ++i) {
        Gamepads[i].available = 0;
        Gamepads[i].controller = 0;
        Gamepads[i].left_x = 0;
        Gamepads[i].left_y = 0;
        Gamepads[i].right_x = 0;
        Gamepads[i].right_y = 0;
        MappedPads[i] = 0;
    }
}

void GamepadHandleEvent( SDL_Event* event) {

    if( event->type == SDL_JOYAXISMOTION ) {
        hw_gamepad* gp = MappedPads[ event->jaxis.which ];
        _dirty = 1;
        if(gp) {
            switch( event->jaxis.axis ) {
                case 0:
                    gp->left_x = (double)event->jaxis.value / 32276.0;
                    break;
                case 1:
                    gp->left_y = (double)event->jaxis.value / 32276.0;
                    break;
                case 2:
                    gp->right_x = (double)event->jaxis.value / 32276.0;
                    break;
                case 3:
                    gp->right_y = (double)event->jaxis.value / 32276.0;
                    break;
            }

        //   printf(" gp(%-20f,%-20f,%-20f,%-20f)\n",gp->left_x,gp->left_y, gp->right_x, gp->right_y );
        }
    }
}

hw_gamepad* getGamepad(int index) {

    return( &Gamepads[index] );
}

void initGamepad() {

    int found = 0;
	int i;

    printf("initializing gamepad system\n");
    resetGamepad();


    printf("   I see %i potential controllers..\n",SDL_NumJoysticks());
    for(i=0; i<SDL_NumJoysticks(); ++i) {
        if(SDL_IsGameController(i)) {
            hw_gamepad *gp = &Gamepads[found]; 
            found++;
            gp->controller = SDL_GameControllerOpen(i);
            gp->index = i;
            gp->available = 1;
            MappedPads[i] = gp;
        }
    }

    printf("initialized %i gamepads\n",found);
}

void dropGamepad() {
    int i;

    printf("dropping gamepad system\n");
    for(i=0; i<MAX_PADS; ++i) {

        hw_gamepad *gp = &Gamepads[i];

        if(gp->available) {
            printf("closing gamepad %i\n", gp->index);
            SDL_GameControllerClose( gp->controller );
        }
    }
}
