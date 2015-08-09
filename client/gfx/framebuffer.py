import client.system.log as log
import hwgfx

#TODO:  this should be populated from the hardware's reported  maximum number 
#       of framebuffer units. 


class framebuffer:
    def __init__(self, fb):
        self._fb   = fb
        log.write( log.DEBUG, "Acquired framebuffer:{0}".format(self._fb))

    def __del__(self):
        hwgfx.framebuffer_drop(self._fb)
        log.write( log.DEBUG, "Dropped framebuffer:{0}".format(self._fb))
        
    @classmethod
    def from_texture(cls,texture):
        fb = hwgfx.framebuffer_create()
        hwgfx.framebuffer_bind_texture( fb, texture._tex )
        return cls(fb)

class framebuffer_as_render_target:
    def __init__(self,framebuffer):
        self.framebuffer = framebuffer

    def __enter__(self):
        hwgfx.framebuffer_render_start( self.framebuffer._fb )

    def __exit__(self, exc_type, exc_value, traceback):
        hwgfx.framebuffer_render_end( self.framebuffer._fb )

