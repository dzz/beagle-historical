import  client.ui.areas
from    client.ui.window import window




def init():
    client.ui.areas.register_ui_area( window( title ="brush editor",x=0,    y=0 )) 
    client.ui.areas.register_ui_area( window( title ="timeline",    x=0,    y=300,   width=800, height=150))
    client.ui.areas.register_ui_area( window( title ="colours",     x=350,  y=300,   width=150, height=150))

def finalize():
    pass
