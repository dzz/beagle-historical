from enum import Enum

_drawmode_map = []

#bindings as per api-includes/modules/hwgfx.h
draw_mode(Enum):
    POINTS          = 0
    LINE_STRIP      = 1
    LINE_LOOP       = 2
    LINES           = 3
    TRI_STRIP       = 4
    TRI_FAN         = 5
    TRIS            = 6

def INIT_set_drawmode_map(hwgfx_map):
    global _drawmode_map
    _drawmode_map = hwgfx_map

