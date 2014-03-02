#ifndef __MAPPER_EDITOR_BANK__
#define __MAPPER_EDITOR_BANK__

#define MAPPER_BANK_HEIGHT (128*3)

#include "panels.h"

typedef struct {
	double min_x;
	double max_x;
	double min_y;
	double max_y;
	unsigned int _idx;
} mapping_function;


void initMapperEditorBank(void);
void destroyMapperEditorBank(void);
void renderMapperEditorBank(SDL_Surface* target,UI_AREA* area);

mapping_function* mapperbank_get_functions(void);
double mapperbank_compute_mapping(mapping_function* function,double input);

#endif
