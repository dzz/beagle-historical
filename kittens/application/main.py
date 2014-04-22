from __future__ import absolute_import

import kittens.ui_area as ui_area
import kittens.ui_window as ui_window

window = None

def init():
    global window
    window = ui_window.ui_window()
    ui_area.register_ui_area(window)
    return

def finalize():
    global window
    window = None

