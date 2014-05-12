from client.ui.window       import window

class layout_flexcolumn:
    def __init__(self, weights=[1,1], height = None):
        total = float(sum(weights))

        self.normalized_weights = []
        for x in weights:
            self.normalized_weights.append( float(x) / total )

        self.num_columns = len(weights)
        self.height = height

    def perform_layout(self, ui_area):
        used_width = 0
        row_height = self.height

        if(row_height is None):
            row_height = ui_area.get_height() // (len(ui_area.children) // self.num_columns)

        for i in range(0, len(ui_area.children)):
            x = i % self.num_columns
            y = i / self.num_columns

            if y == 0:
                used_width = 0

            width = self.normalized_weights[x] * ui_area.get_width()
            used_width += width

            child = ui_area.children[i]
            child.set_x(used_width)
            child.set_y( y * row_height)
            child.set_width(width)
            child.set_height(row_height)



class property_window(window):
    def __init__():
        window.init("properties",x=0,y=0,width=400,height=400);

        self.add_layout( layout_flexcolumn(2, 24) )


