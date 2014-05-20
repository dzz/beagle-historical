from client.ui.window               import window
from client.ui.layout_flexcolumn    import layout_flexcolumn
from client.ui.text_box             import text_box

class layout_rowborders:
    def __init__(self, row_border = 5):
        self.row_border = row_border

    def perform_layout(self,ui_area):
        for child in ui_area.children:
            child.set_height( child.get_height() - self.row_border );

class property_window(window):
    def __init__(self, property_map = {"default": ""} ):
        window.__init__(self,"properties",x=0,y=0,width=400,height=400);

        for k in property_map.keys():
            self.add_child( text_box( text=k ) )
            self.add_child( text_box( text=property_map[k], editable=True, use_python=True ) )

        self.add_layout( layout_flexcolumn(weights = [1,3], row_height = None) )
        self.add_layout(layout_rowborders() )



