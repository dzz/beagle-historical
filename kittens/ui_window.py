from ui_area import *
from area_renderer import area_renderer
from mod_empty import *
from mod_resize import *
from mod_titlebar import *
from mod_parent import *

class window_renderer(area_renderer):
    def __init__(self,ui_area):
        self.label = gfx.label( text        = ui_area.prop["titlebar_text"], 
                                color       = style.get("window_titlebar_text_color"),
                                fontsize    = style.get("window_titlebar_fontsize"))

        self.title_bgcol    = style.get("window_titlebar_bg_color")
        self.bgcol          = style.get("window_bg_color")
        self.clientcol      = style.get("window_client_color")

    def render(self, ui_area):
        gfx.solid_rect( ui_area.r, self.bgcol)
        gfx.solid_rect( [ui_area.r[0],
                         ui_area.r[1],
                         ui_area.r[2], 
                         ui_area.prop.get("titlebar_height")],
                         self.title_bgcol) 
        self.label.draw(ui_area.r[0], ui_area.r[1])
        gfx.solid_rect( ui_area.compute_client_area(), self.clientcol)

class ui_window(ui_area):
    def __init__(self,title="ctt2_window",x=200,y=200,width=200,height=200):
        ui_area.__init__(self)
        self.r = [x,y,width,height]
        self.modifier_stack = [
                               mod_resize   ( handle_size = 9 ),
                               mod_titlebar ( self, titlebar_text = title, height = 12 ),
                               mod_parent   ( )]

        self.renderers = [ window_renderer(self) ]
