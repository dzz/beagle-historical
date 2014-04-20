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
#define LABEL_AZIMUTH 22
#define LABEL_ORIENT 23
#define LABEL_ROT 24
#define NUM_LABELS 25


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
	labels[LABEL_BRUSH_CONTROLLER]=IMG_Load("c:/c/ctt2/ui_gen/generated/title_brush.bmp");\
	labels[LABEL_SIZE]=IMG_Load("ui_gen/generated/title_size.bmp");\
	labels[LABEL_COLOR]=IMG_Load("ui_gen/generated/title_color.bmp");\
	labels[LABEL_ALPHA]=IMG_Load("ui_gen/generated/title_alpha.bmp");\
	labels[LABEL_JITTER]=IMG_Load("ui_gen/generated/title_jitter.bmp");\
	labels[LABEL_NOISE]=IMG_Load("ui_gen/generated/title_noise.bmp");\
	labels[LABEL_STYLUS]=IMG_Load("ui_gen/generated/title_stylus.bmp");\
	labels[LABEL_PRESSURE]=IMG_Load("ui_gen/generated/title_pres.bmp");\
	labels[LABEL_TIME]=IMG_Load("ui_gen/generated/title_time.bmp");\
	labels[LABEL_MAPPER]=IMG_Load("ui_gen/generated/title_mapper.bmp");\
	labels[LABEL_X]=IMG_Load("ui_gen/generated/title_in.bmp");\
	labels[LABEL_Y]=IMG_Load("ui_gen/generated/title_out.bmp");\
	labels[LABEL_R]=IMG_Load("ui_gen/generated/title_r.bmp");\
	labels[LABEL_G]=IMG_Load("ui_gen/generated/title_g.bmp");\
	labels[LABEL_B]=IMG_Load("ui_gen/generated/title_b.bmp");\
	labels[LABEL_A]=IMG_Load("ui_gen/generated/title_a.bmp");\
	labels[LABEL_ADD]=IMG_Load("ui_gen/generated/title_add.bmp");\
	labels[LABEL_MUL]=IMG_Load("ui_gen/generated/title_mul.bmp");\
	labels[LABEL_COLOR]=IMG_Load("ui_gen/generated/title_color.bmp");\
	labels[LABEL_GRADIENT]=IMG_Load("ui_gen/generated/title_grad.bmp");\
	labels[LABEL_ADD3]=IMG_Load("ui_gen/generated/title_add3.bmp");\
	labels[LABEL_MUL3]=IMG_Load("ui_gen/generated/title_mul3.bmp");\
	labels[LABEL_AZIMUTH]=IMG_Load("ui_gen/generated/title_azimuth.bmp");\
	labels[LABEL_ROT]=IMG_Load("ui_gen/generated/title_rot.bmp");\
	labels[LABEL_ORIENT]=IMG_Load("ui_gen/generated/title_orient.bmp");
#endif
