import host
from client.ui.property_window      import property_window
import client.ui.controlled_eval


class python_window(property_window):
    def __init__(self):
        property_window.__init__(self, title="python", property_map = {"input":""} )

        self.set_width(600)
        self.set_height(host.get_char_dims() + self.titlebar_height + (self.children[0].padding*2) )


