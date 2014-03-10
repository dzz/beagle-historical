#ifndef __NODE_MAP_EDITOR__
#define __NODE_MAP_EDITOR__

void initNodeMapEditor();
void destroyNodeMapEditor();

void renderNodeMapEditor(SDL_Surface* target, UI_AREA* area);

void nodemapeditor_mousedown(int x,int y);
void nodemapeditor_mouseup(int x,int y);
void nodemapeditor_mousemotion(int x, int y);
void nodemapeditor_mouseleave(void);

#endif
