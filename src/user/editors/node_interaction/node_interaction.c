#include "../../../drawing/node_resource_ids.h"

#include "node_interaction.h"

typedef struct {
	void (*mousedown)(mapper_node*,int,int);
	void (*mousemotion)(mapper_node*,int,int);
	void (*mouseup)(mapper_node*);
} node_dispatcher;

node_dispatcher node_dispatchers[TOTAL_INTERFACES] = {0};

#define IMPORT_INTERFACE(x)\
node_dispatchers[NODE_INTERFACE_##x].mousedown=&node_mousedown_##x;\
node_dispatchers[NODE_INTERFACE_##x].mouseup=&node_mouseup_##x;\
node_dispatchers[NODE_INTERFACE_##x].mousemotion=&node_mousemotion_##x;

void initNodeDispatcher() {
	IMPORT_INTERFACE(MAPPER)
	IMPORT_INTERFACE(COLOR)
}

void node_mousedown(mapper_node* node, int cmx, int cmy) {
		if( node_dispatchers[node->interface_type].mousedown!=0 ) {
				node_dispatchers[node->interface_type].mousedown(node,cmx,cmy);
		}
}
void node_mouseup(mapper_node* node){
		if( node_dispatchers[node->interface_type].mouseup!=0 ) {
				node_dispatchers[node->interface_type].mouseup(node);
		}
}
void node_mousemotion(mapper_node* node, int cmx,int cmy) {
		if( node_dispatchers[node->interface_type].mousemotion!=0 ) {
				node_dispatchers[node->interface_type].mousemotion(node,cmx,cmy);
		}
}

