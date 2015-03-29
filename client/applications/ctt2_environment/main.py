from    client.ui.brush_editor_window     import brush_editor_window
from    client.ui.window            import window
from    client.ui.color_window      import color_window

import  client.ui.areas  as areas


def init():
        areas.register_ui_area( brush_editor_window() )

def tick():
    pass

def finalize():
    pass

def render():
    pass
