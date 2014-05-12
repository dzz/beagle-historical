import  client.ui.areas  as areas
from    client.ui.property_window import property_window

def init():
    areas.register_ui_area( property_window() )

def finalize():
    pass
