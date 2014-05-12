from client.ui.areas            import ui_area
from client.ui.window           import window
from client.gfx.label           import label
from client.ui.layout_grid      import layout_grid
from client.ui.default_renderer import default_renderer
from client.gfx.rect            import rect_vgrad

import client.ui.style      as style

class layout_flexcolumn:
    def __init__(self, weights=[1,1], row_height = None):
        total = float(sum(weights))

        self.normalized_weights = []
        for x in weights:
            self.normalized_weights.append( float(x) / total )

        self.num_columns = len(weights)
        self.row_height = row_height

    def perform_layout(self, ui_area):
        used_width = 0
        row_height = self.row_height

        if(row_height is None):
            row_height = ui_area.get_height() // (len(ui_area.children) // self.num_columns)

        for i in range(0, len(ui_area.children)):
            x = i % self.num_columns
            y = i // self.num_columns

            if x == 0:
                used_width = 0

            width = int(self.normalized_weights[x] * ui_area.get_width())

            child = ui_area.children[i]
            child.set_x(used_width)
            child.set_y( y * row_height)
            child.set_width(width)
            child.set_height(row_height)

            used_width += width

class text_box(ui_area):
    def __init__(self, text="", shaded = False ):
        ui_area.__init__(self)
        self.label_color = style.get("default_label_color");
        self.label = label(text)
        self.client_color   = style.get("window_client_color")
        self.renderers = [ default_renderer() ]
        self.shaded = shaded

    def set_text(text):
        self.label.set_text(text)

    def render_client_area(self):
        if(self.shaded): 
            rect_vgrad  ( [0,0,self.r[2],self.r[3]], self.client_color[0],self.client_color[1])
        self.label.draw(0,0, self.label_color)

class property_window(window):
    def __init__(self):
        window.__init__(self,"properties",x=0,y=0,width=400,height=400);

        property_map = {}
        property_map["color"] = "[1,2,3]"
        property_map["evil"]  = "666"

        for rep in range(0,6):
            for k in property_map.keys():
                self.add_child( text_box( text=k ) )
                self.add_child( text_box( text=property_map[k], shaded=True ) )
        
        self.add_layout( layout_flexcolumn(weights = [1,3], row_height = 24) )
        #self.add_layout( layout_grid(2,2) )


