#ifndef __NODE_RESOURCE_IDS__
#define __NODE_RESOURCE_IDS__

#define LABEL_BRUSH_CONTROLLER 0
#define LABEL_SIZE 1
#define LABEL_COLOR 2
#define LABEL_ALPHA 3
#define LABEL_JITTER 4
#define LABEL_NOISE 5
#define LABEL_STYLUS 6
#define LABEL_PRESSURE 7
#define LABEL_TIME 8
#define LABEL_MAPPER 9
#define LABEL_X 10
#define LABEL_Y 11
#define LABEL_ADD 12
#define LABEL_R 13
#define LABEL_G 14
#define LABEL_B 15
#define LABEL_A 16
#define LABEL_MUL 17
//#define LABEL_COLOR 18
#define LABEL_GRADIENT 19
#define LABEL_ADD3 20
#define LABEL_MUL3 21
#define NUM_LABELS 22


#define TEMPLATE_MAPPER 0
#define TEMPLATE_ADD 1
#define TEMPLATE_MUL 2
#define TEMPLATE_COLOR 3
#define TEMPLATE_GRADIENT 4
#define TEMPLATE_NOISE 5
#define TEMPLATE_OSCILLATOR 5
#define TEMPLATE_SLIDER 6
#define TEMPLATE_ADD3 7

#define NODE_INTERFACE_NONE 0
#define NODE_INTERFACE_MAPPER 1
#define NODE_INTERFACE_COLOR 2
#define NODE_INTERFACE_GRADIENT 3
#define TOTAL_INTERFACES 4


#define LOAD_NODE_IMAGE_RESOURCES \
	labels[LABEL_BRUSH_CONTROLLER]=SDL_LoadBMP("ui_gen/generated/title_brush.bmp");\
	labels[LABEL_SIZE]=SDL_LoadBMP("ui_gen/generated/title_size.bmp");\
	labels[LABEL_COLOR]=SDL_LoadBMP("ui_gen/generated/title_color.bmp");\
	labels[LABEL_ALPHA]=SDL_LoadBMP("ui_gen/generated/title_alpha.bmp");\
	labels[LABEL_JITTER]=SDL_LoadBMP("ui_gen/generated/title_jitter.bmp");\
	labels[LABEL_NOISE]=SDL_LoadBMP("ui_gen/generated/title_noise.bmp");\
	labels[LABEL_STYLUS]=SDL_LoadBMP("ui_gen/generated/title_stylus.bmp");\
	labels[LABEL_PRESSURE]=SDL_LoadBMP("ui_gen/generated/title_pres.bmp");\
	labels[LABEL_TIME]=SDL_LoadBMP("ui_gen/generated/title_time.bmp");\
	labels[LABEL_MAPPER]=SDL_LoadBMP("ui_gen/generated/title_mapper.bmp");\
	labels[LABEL_X]=SDL_LoadBMP("ui_gen/generated/title_in.bmp");\
	labels[LABEL_Y]=SDL_LoadBMP("ui_gen/generated/title_out.bmp");\
	labels[LABEL_R]=SDL_LoadBMP("ui_gen/generated/title_r.bmp");\
	labels[LABEL_G]=SDL_LoadBMP("ui_gen/generated/title_g.bmp");\
	labels[LABEL_B]=SDL_LoadBMP("ui_gen/generated/title_b.bmp");\
	labels[LABEL_A]=SDL_LoadBMP("ui_gen/generated/title_a.bmp");\
	labels[LABEL_ADD]=SDL_LoadBMP("ui_gen/generated/title_add.bmp");\
	labels[LABEL_MUL]=SDL_LoadBMP("ui_gen/generated/title_mul.bmp");\
	labels[LABEL_COLOR]=SDL_LoadBMP("ui_gen/generated/title_color.bmp");\
	labels[LABEL_GRADIENT]=SDL_LoadBMP("ui_gen/generated/title_grad.bmp");\
	labels[LABEL_ADD3]=SDL_LoadBMP("ui_gen/generated/title_add3.bmp");\
	labels[LABEL_MUL3]=SDL_LoadBMP("ui_gen/generated/title_mul3.bmp");
#endif
