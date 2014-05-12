import  client.ui.areas
from    client.ui.window import window

def init():
    bwin = window()
    bwin.add_child(window())

    pwin = window( title ="brush editor",x=0,y=0, width=400,height=400 );
    pwin.add_child( bwin )
    pwin.add_child( window() )

    client.ui.areas.register_ui_area( pwin ) 

def finalize():
    pass
