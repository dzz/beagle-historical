#ifndef __LAYERS__
#define __LAYERS__

#define MAX_LAYERS 4
#define MAX_LAYERNAME_SIZE 128

typedef struct {
	int mode;
	char name [MAX_LAYERNAME_SIZE];
} layer;

extern layer layers[MAX_LAYERS];
extern unsigned int numLayers;

void initLayers(void ); 
unsigned int getActiveLayer(void);

void setActiveLayer(unsigned int layer);

#endif
