from    client.ui.property_window   import property_window
from    client.ui.window            import window
from    client.ui.color_window      import color_window

import  client.ui.areas  as areas


def init():
        areas.register_ui_area( property_window() )

        pwin = window( title = "parent", width = 400, height=400)
        cwin = window( title = "child", x = 25, y = 25, width=100, height=100)
        pwin.add_child(cwin);
        pwin.add_child(property_window() )
        areas.register_ui_area( property_window() )

        areas.register_ui_area( pwin )
        areas.register_ui_area( color_window() )

def finalize():
    pass
