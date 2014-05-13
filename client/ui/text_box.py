from client.ui.areas                import ui_area
from client.gfx.rect            import rect_vgrad
from client.gfx.label           import label
from client.ui.default_renderer     import default_renderer

import client.ui.style      as style

class text_box(ui_area):
    def __init__(self, text="", shaded = False ):
        global _tmplabel
        ui_area.__init__(self)
        self.label_color = style.get("default_label_color");
        self.text = text
        self.label = label(self.text)
        self.client_color   = style.get("window_client_color")
        self.renderers = [ default_renderer() ]
        self.shaded = shaded

    def set_text(self, text):
        self.text = text
        self.label.set_text(text)

    def render_client_area(self):
        if(self.shaded): 
            rect_vgrad  ( [0,0,self.r[2],self.r[3]], self.client_color[0],self.client_color[1])
        self.label.draw(0,0, self.label_color)
