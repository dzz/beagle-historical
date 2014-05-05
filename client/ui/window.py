import host
from client.ui.areas            import ui_area
from client.ui.mod_empty        import mod_empty
from client.ui.mod_resize       import mod_resize
from client.ui.mod_titlebar     import mod_titlebar
from client.ui.mod_parent       import mod_parent
from client.ui.window_renderer  import window_renderer
from client.ui.child_renderer   import child_renderer
from client.gfx.rect            import rect_solid

class window(ui_area):
    def __init__(self,title="ctt2_window",x=10,y=10,width=75,height=75):
        ui_area.__init__(self)
        self.r = [x,y,width,height]
        self.modifier_stack = [
                               mod_resize   ( handle_size = 9 ),
                               mod_titlebar ( self, titlebar_text = title, height = 12 ),
                               mod_parent   ( )
                               ]

        self.renderers = [ window_renderer(self),
                           child_renderer(self) ]

    def render_client_area(self):
        mr = [ self.client_m_pos[0], self.client_m_pos[1], 15,15 ]
        rect_solid( mr, [1.0,0.0,0.0,1.0] )


class mouse_shower(ui_area):
    def __init__(self,x=125,y =15,width=50,height=50):
        self.r = [x,y,width,height]
        ui_area.__init__(self)

    def render_client_area(self):
        host.abort()
        mr = [ self.client_m_pos[0], self.client_m_pos[1], 15,15 ]
        rec_solid(self.r, [0.0,1.0,0.0,1.0])
        rect_solid( mr, [1.0,0.0,0.0,1.0] )
