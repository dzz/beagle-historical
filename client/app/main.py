import  client.ui.areas  as areas
from    client.ui.property_window import property_window

def out_of_scope():
    tmp = property_window()

def init():
    for i in range(0,3):
        out_of_scope()
        tmp = property_window()
        areas.register_ui_area( tmp )
        areas.remove_ui_area( tmp )
        tmp = property_window()
        areas.register_ui_area( tmp )
        areas.remove_ui_area(tmp)
        areas.register_ui_area( tmp )

def finalize():
    pass
