import hwgfx
from enum import IntEnum

_drawmode_map = []

#bindings as per api-includes/modules/hwgfx.h
class draw_mode(IntEnum):
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
    for mode in _drawmode_map:
        print("PY: loaded drawing mode:", mode)

class primitive:
    def __init__(self,mode, floats_per_vertex, coords, uvs = None ):
        global _drawmode_map
        if uvs is None:
            self._prim = hwgfx.primitive_create_coordinate_primitive( 
                    coords, 
                    _drawmode_map[mode])
            self._has_uvs = False
        else:
            self._prim = hwgfx.primitive_create_coordinate_uv_primitive(
                    coords, uvs,
                    floats_per_vertes,
                    _drawmode_map[mode])
            self._has_uvs = True
        print("PY: acquired primitive ", self._prim, " uvs:", self._has_uvs)

    def __del__(self):
        if self._has_uvs:
            hwgfx.primitive_destroy_coordinate_uv_primitive( self._prim )
        else:
            hwgfx.primitive_create_coordinate_primitive( self._prim )
        print("PY: dropped primitive ", self._prim, " uvs:", self._has_uvs)

    def render(self):
        hwgfx.primitive_render(self._prim)

