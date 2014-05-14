from    client.ui.property_window   import property_window
from    client.ui.window            import window
from    client.ui.color_window      import color_window

import  client.ui.areas  as areas


def init():
        areas.register_ui_area( property_window() )
        areas.register_ui_area( color_window() )

def finalize():
    pass
