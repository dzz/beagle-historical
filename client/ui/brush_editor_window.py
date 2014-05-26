from client.ui.areas                import ui_area
from client.ui.property_editor      import property_editor
from client.ui.window               import window
from client.ui.layout_flexcolumn    import layout_flexcolumn
from client.ui.default_renderer     import default_renderer
from client.gfx.rect            import rect_vgrad

class node_editor(ui_area):
    def __init__(self):
        ui_area.__init__(self)
        self.renderers = [ default_renderer() ]
    
    def render_client_area(self):
        rect_vgrad  ( [0,0,self.r[2],self.r[3]], [1.0,1.0,1.0,1.0],[0.5,0.5,0.6,1.0] )


class brush_editor_window(window):
    def __init__(self):

        property_map = {"color" :"[0.0,0.0,0.0]"}

        window.__init__(self,title="brush editor" ,x=0,y=0,width=600,height=400)
        self.add_child( property_editor( property_map ) )
        self.add_child( node_editor() )
        self.add_layout( layout_flexcolumn( weights=[2.2,5] ) )

