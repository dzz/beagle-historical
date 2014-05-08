import  client.ui.areas
from    client.ui.window import window
from    client.ui.window import mouse_shower

def init():
    pwin = window( title ="brush editor",x=0,y=0, width=400,height=400 );
    pwin.add_child( window() )
    pwin.add_child( mouse_shower() )
    client.ui.areas.register_ui_area( pwin ) 
    #client.ui.areas.register_ui_area( window( title ="timeline",    x=0,    y=300,   width=800, height=150))
    #client.ui.areas.register_ui_area( window( title ="colours",     x=350,  y=300,   width=150, height=150))
   
def finalize():
    pass
