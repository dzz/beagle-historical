#ifndef __COMPOSITOR__
#define __COMPOSITOR__

#include <SDL.h>

#define COMPOSITOR_DEFAULT_W 1920
#define COMPOSITOR_DEFAULT_H 1080

typedef struct {
	  SDL_Surface *data;
	  int w;
	  int h;
	  int mode; 
	  int x;
	  int y;
} COMPOSITE_LAYER;

extern const int CMP_ADD;
extern const int CMP_SUB;
extern const int CMP_SRC;
extern const int CMP_ZERO;
extern const int CMP_ALPHA_OVER;

typedef SDL_Rect COMPOSITE_AREA;
typedef SDL_Surface * COMPOSITE_FLAT;
typedef COMPOSITE_FLAT COMPOSITE;

COMPOSITE compositeLayers(COMPOSITE_LAYER *stack,int len, COMPOSITE_AREA area);
void deleteComposite(COMPOSITE composite);
COMPOSITE_LAYER createCompositeLayer(int w,int h);
COMPOSITE_LAYER* allocateCompositeLayer(int w,int h);

void destroyCompositeLayer(COMPOSITE_LAYER layer);
unsigned char comp_alpha_over(unsigned char l, unsigned char r,unsigned char src_a);
void initCompositor(void);

#endif
