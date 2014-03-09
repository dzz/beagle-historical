#include <SDL.h>
#include "panels.h"
#include "nodeMapEditor.h"
#include "../drawing/node_mapper.h"

void initNodeMapEditor(){

}

void destroyNodeMapEditor(){

}

void renderNodeMapEditor(SDL_Surface* target, UI_AREA* area){
		unsigned int bg_color = SDL_MapRGB(target->format,
						16,16,16);
		unsigned int node_color = SDL_MapRGB(target->format,
						64,64,64);

		SDL_FillRect(target, (SDL_Rect*) area, bg_color);

		{
			int i;
			mapper_node** nodes = nodemapper_get_node_array();
			for(i=0; i<MAX_NODES; ++i) 
			if(nodes[i]==0) break; else	
			{
				SDL_Rect r;
				r.x = nodes[i]->x+area->x;
				r.y = nodes[i]->y+area->y;
				r.w = 100;
				r.h = 100;
				SDL_FillRect(target, &r, node_color);
			}
		}
}

void nodemapeditor_mousedown(int x,int y, UI_AREA* area){

}
