from    client.ui.python_window     import python_window
from    client.ui.window            import window
from    client.ui.color_window      import color_window

import  client.ui.areas  as areas


def init():
        #properties = {"my key": "", "what": "strval", "other" : "10.0" };
        #areas.register_ui_area( property_window( "python eval",properties ) )

        areas.register_ui_area( python_window() )
        #areas.register_ui_area( color_window() )

def finalize():
    pass
