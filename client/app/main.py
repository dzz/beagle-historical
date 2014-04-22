import  client.ui.areas
from    client.ui.window import window


def init():
    client.ui.areas.register_ui_area( window() )

def finalize():
    pass
