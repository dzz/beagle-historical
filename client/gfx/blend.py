import hwgfx

mode_over              = 0
mode_brush_composite   = 1
mode_dab_rendering     = 2
mode_blackhole         = 600

class state:
    def __init__(self,mode):
        self.mode = mode

    def __enter__(self):
        hwgfx.blend_enter(self.mode)

    def __exit__(self, exc_type, exc_value, traceback):
        hwgfx.blend_exit()

