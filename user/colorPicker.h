#ifndef __COLOR_PICKER__
#define __COLOR_PICKER__

#define COLORPICKER_WIDTH 128
#define COLORPICKER_HEIGHT 256

#include "panels.h"
#include <SDL.H>

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} cp_color;

void initColorPicker(void);
void destroyColorPicker(void);

void renderColorPicker(SDL_Surface *target, UI_AREA *area);

void colorpicker_mousedown(int x,int y, UI_AREA *area);
void colorpicker_mouseup(int x,int y, UI_AREA *area);
void colorpicker_mousemotion(int x,int y, UI_AREA *area);
void colorpicker_mouseleave();

cp_color getCurColor(void);

cp_color getPrimaryColor(void);
cp_color getSecondaryColor(void);

void cp_toggle_primary_secondary(void);
unsigned int get_cp_secondary(void);

#endif
