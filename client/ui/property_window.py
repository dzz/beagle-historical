from client.ui.window               import window
from client.ui.layout_flexcolumn    import layout_flexcolumn
from client.ui.text_box             import text_box



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
        
        self.add_layout( layout_flexcolumn(weights = [1,3], row_height = None) )



