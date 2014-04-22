from client.ui.areas            import ui_area
from client.ui.mod_empty        import mod_empty
from client.ui.mod_resize       import mod_resize
from client.ui.mod_titlebar     import mod_titlebar
from client.ui.mod_parent       import mod_parent
from client.ui.window_renderer  import window_renderer

class window(ui_area):
    def __init__(self,title="ctt2_window",x=200,y=200,width=200,height=200):
        ui_area.__init__(self)
        self.r = [x,y,width,height]
        self.modifier_stack = [
                               mod_resize   ( handle_size = 9 ),
                               mod_titlebar ( self, titlebar_text = title, height = 12 ),
                               mod_parent   ( )]

        self.renderers = [ window_renderer(self) ]


