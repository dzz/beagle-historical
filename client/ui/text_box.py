from client.ui.areas                import ui_area
from client.gfx.rect                import rect_solid
from client.gfx.rect                import rect_vgrad
from client.gfx.label               import label
from client.ui.default_renderer     import default_renderer
from client.ui.mod_caret_handler    import mod_caret_handler
from client.ui.mod_text_editor      import mod_text_editor

import client.ui.style      as style

class text_box(ui_area):
    def __init__(self, text="", editable = False, padding = 4 ):
        global _tmplabel
        ui_area.__init__(self)
        self.label_color = style.get("default_label_color");
        self.text = text
        self.label = label(self.text)
        self.client_color   = style.get("window_client_color")
        self.renderers = [ default_renderer() ]
        self.editable = editable
        self.active_color = style.get("active_textbox_color")
        self.padding = padding

        if self.editable:
            self.add_modifier(mod_caret_handler())
            self.add_modifier(mod_text_editor())

    def set_text(self, text):
        self.text = text
        self.label.set_text(text)

    def get_text(self):
        return self.text

    def render_client_area(self):
        pad_px = self.padding
        if(self.editable): 
            if(self.has_caret()):
                rect_solid  ( self.get_dims() , self.active_color )
        else:
            rect_vgrad( self.get_dims(), self.client_color[0], self.client_color[1] ) 
        self.label.draw(pad_px,pad_px, self.label_color)
