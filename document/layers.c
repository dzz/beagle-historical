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

	strcpy_s(layers[0].name, MAX_LAYERNAME_SIZE, colName);
	strcpy_s(layers[1].name, MAX_LAYERNAME_SIZE, lineName);

	numLayers = 2;
	activeLayer = 1;
}

unsigned int getActiveLayer(void) {
	return activeLayer;
}

void setActiveLayer(unsigned int layer) {
	activeLayer = layer;
}
