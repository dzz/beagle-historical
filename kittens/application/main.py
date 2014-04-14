from __future__ import absolute_import

import kittens.ui_area as ui_area
import kittens.ui_window as ui_window

def init():
    window = ui_window.ui_window()
    ui_area.register_ui_area(window)
    return

