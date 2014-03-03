#ifndef __MAPPER_EDITOR_BANK__
#define __MAPPER_EDITOR_BANK__

#define MAPPER_BANK_HEIGHT 100

#define MAPPER_COLOR 0
#define MAPPER_ALPHA 1
#define MAPPER_SIZE 2
#define MAPPER_JITTER 3
#define MAPPER_NOISE 4

#include "panels.h"

typedef struct {
	double min_x;
	double max_x;
	double min_y;
	double max_y;
	unsigned int _idx;
	unsigned int _min_render_x;
	unsigned int _min_render_y;
	unsigned int _max_render_x;
	unsigned int _max_render_y;
} mapping_function;


void initMapperEditorBank(void);
void destroyMapperEditorBank(void);
void renderMapperEditorBank(SDL_Surface* target,UI_AREA* area);

mapping_function* mapperbank_get_functions(void);
double mapperbank_compute_mapping(mapping_function* function,double input);
double mapperbank_get_mapping(int idx,double input);

void mapperbank_mousedown(int x, int y, UI_AREA *area);
void mapperbank_mouseup(int x, int y, UI_AREA *area);
void mapperbank_mousemotion(int x, int y, UI_AREA *area);
void mapperbank_mouseleave();

#endif
