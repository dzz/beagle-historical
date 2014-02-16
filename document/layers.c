#include "../compositor/compositor.h"
#include "layers.h"
#include <string.h>

layer layers[MAX_LAYERS];
unsigned int numLayers;
unsigned int activeLayer;

void initLayers(void) {
	char * colName = "colors";
	char * lineName = "lines";

	layers[0].mode = CMP_ADD;
	layers[1].mode = CMP_ALPHA_OVER;

	strcpy(layers[0].name, colName);
	strcpy(layers[1].name, lineName);

	numLayers = 2;
	activeLayer = 1;
}

unsigned int getActiveLayer(void) {
	return activeLayer;
}

void setActiveLayer(unsigned int layer) {
	activeLayer = layer;
}
