from client.ui.areas                import ui_area
from client.ui.property_editor      import property_editor
from client.ui.window               import window
from client.ui.layout_flexcolumn    import layout_flexcolumn
from client.ui.layout_flexrow       import layout_flexrow
from client.ui.mod_parent           import mod_parent
from client.ui.child_renderer       import child_renderer
from client.ui.default_renderer     import default_renderer
from client.gfx.rect                import rect_vgrad
from client.ui.panel                import panel


class node_connection:
    node    = None
    channel = None

class node:
    id = 0
    name = "passthru"
    input_names     = ["in"]       
    output_names    = ["out"]
    input_cons      = []
    output_cons     = []

class node_window(window):
    node = node()

    def __init__(self):
        window.__init__(self, title=node.name )

class node_editor(ui_area):
    def __init__(self):
        ui_area.__init__(self)
        self.renderers = [ default_renderer(), child_renderer(self) ]
        self.add_modifier( mod_parent() )
        self.add_child(node_window()) 

    def render_client_area(self):
        rect_vgrad  ( [0,0,self.r[2],self.r[3]], [1.0,1.0,1.0,1.0],[0.5,0.5,0.6,1.0] )

class brush_editor_window(window):
    def __init__(self):

        property_map = {"color" :"[0.0,0.0,0.0]", "test":"hello world"}

        window.__init__(self,title="brush editor" ,x=0,y=0,width=600,height=400)

        npanel = panel()
        npanel.add_child( node_editor() )

        self.add_child( npanel )
        self.add_child( property_editor( property_map ) )
        self.add_child( node_editor() )
        self.add_child( property_editor( property_map ) )

        self.add_layout( layout_flexrow( weights=[2, 4, 4,2] ) )

