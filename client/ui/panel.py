
import  client.ui.style             as style
from client.ui.areas                import ui_area
from client.ui.child_renderer       import child_renderer
from client.ui.mod_parent           import mod_parent
from client.ui.default_renderer     import default_renderer
from client.gfx.label               import label 


class panel(ui_area):
    def __init__(self):
        ui_area.__init__(self)
        self.modifier_stack = [ mod_parent() ]
        self.renderers      = [ default_renderer(), child_renderer(self)]
        self.label = label( text        = "panel",
                            fontsize    = style.get("window_titlebar_fontsize"))

    def render_client_area(self):
        self.label.draw(0,0,[0.0,0.0,0.0])
