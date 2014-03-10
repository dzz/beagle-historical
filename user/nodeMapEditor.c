#include <SDL.h>
#include "panels.h"
#include "nodeMapEditor.h"
#include "../drawing/node_mapper.h"

static SDL_Surface* labels[NUM_LABELS] = {0};

void initNodeMapEditor(){
	labels[LABEL_BRUSH_CONTROLLER]=SDL_LoadBMP("ui_gen/title_brush_controller.bmp");
	labels[LABEL_SIZE]=SDL_LoadBMP("ui_gen/title_size.bmp");
	labels[LABEL_COLOR]=SDL_LoadBMP("ui_gen/title_color.bmp");
	labels[LABEL_ALPHA]=SDL_LoadBMP("ui_gen/title_alpha.bmp");
	labels[LABEL_JITTER]=SDL_LoadBMP("ui_gen/title_jitter.bmp");
	labels[LABEL_NOISE]=SDL_LoadBMP("ui_gen/title_noise.bmp");
	labels[LABEL_STYLUS]=SDL_LoadBMP("ui_gen/title_stylus.bmp");
	labels[LABEL_PRESSURE]=SDL_LoadBMP("ui_gen/title_pressure.bmp");
	labels[LABEL_TIME]=SDL_LoadBMP("ui_gen/title_time.bmp");
}

void destroyNodeMapEditor(){
	int i;
	for(i=0; i< NUM_LABELS; ++i) {
		if(labels[i] != 0)
				SDL_FreeSurface(labels[i]);
	}
}

int get_node_gui_height(mapper_node* node) {
	return 0;
}

int usable_input_channels(mapper_node* node) {
	if( node->binding_mode != BINDING_MODE_INPUT_DRIVER ) 
			return node->input_channels; else
					return 0;
}

int usable_output_channels(mapper_node* node) {
	if( node->binding_mode != BINDING_MODE_OUTPUT_DRIVER ) 
			return node->output_channels; else
					return 0;
}

void renderNodeMapEditor(SDL_Surface* target, UI_AREA* area){
		unsigned int channel_input_color = SDL_MapRGB(target->format,
						40,40,40);
		unsigned int channel_output_color = SDL_MapRGB(target->format,
						99,99,99);
		unsigned int bg_color = SDL_MapRGB(target->format,
						16,16,16);

		unsigned int title_color = SDL_MapRGB(target->format,
						72,72,72);
		unsigned int node_color = SDL_MapRGB(target->format,
						64,64,64);

		SDL_FillRect(target, (SDL_Rect*) area, bg_color);

		{
			int i;
			mapper_node** nodes = nodemapper_get_node_array();
			for(i=0; i<MAX_NODES; ++i) 
			if(nodes[i]==0) break; else	
			{
				const int channel_height = 16;
				const int node_titlebar_height = 16;
				int gui_height = get_node_gui_height(nodes[i]);
				SDL_Rect r;
				int input_channels = usable_input_channels(nodes[i]);
				int output_channels = usable_output_channels(nodes[i]);

				r.x = nodes[i]->x+area->x;
				r.y = nodes[i]->y+area->y;
				r.w = 200 + 16;
				r.h =(  input_channels + output_channels ) * channel_height;
				r.h = r.h < gui_height ? gui_height : r.h;

				SDL_FillRect(target, &r, node_color);
				{	
					SDL_Rect titlebar = r;

					titlebar.h = node_titlebar_height;
					titlebar.y -= titlebar.h;
					SDL_FillRect(target, &titlebar, title_color);
					SDL_BlitSurface( labels[ nodes[i]->node_label ], NULL, target, &titlebar );
				}

				{
						const int label_w = 100;
						const int label_h = 16;
						const int channel_pin_size = 12;
						const int channel_border = (channel_height-channel_pin_size) / 2;
						int j;

						for(j=0; j< input_channels;++j) {
								SDL_Rect chan_rect;
								chan_rect.x = r.x + ( channel_border  );
								chan_rect.y = r.y + ( channel_height*j ) + ( channel_border );
								chan_rect.w = channel_pin_size;
								chan_rect.h = channel_pin_size;
								SDL_FillRect(target, &chan_rect, channel_input_color );

								chan_rect.x += channel_pin_size;
								chan_rect.y -= channel_border;
								chan_rect.w = label_w;
								SDL_BlitSurface( labels[ nodes[i]->input_labels[j] ],
												NULL, target, &chan_rect );
						}
						for(j=0; j< output_channels;++j) {
								SDL_Rect chan_rect;
								chan_rect.x = (r.x + r.w) - channel_height + channel_border;
								chan_rect.y = (channel_height * input_channels) + r.y + ( channel_height*j ) + (channel_border);

								chan_rect.w = channel_pin_size;
								chan_rect.h = channel_pin_size;
								SDL_FillRect(target, &chan_rect, channel_output_color );

								chan_rect.x -= channel_pin_size + label_w;
								chan_rect.y -= channel_border;
								chan_rect.w = label_w;
								SDL_BlitSurface( labels[ nodes[i]->output_labels[j] ],
												NULL, target, &chan_rect );

						}
				}
			}
		}
}

void nodemapeditor_mousedown(int x,int y, UI_AREA* area){

}
