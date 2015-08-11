import client.system.log as log
import hwgfx

#TODO:  this should be populated from the hardware's reported  maximum number 
#       of framebuffer units. 


class framebuffer:
    stack = []
    def __init__(self, fb, texture):
        self._fb   = fb
        #we grab a reference to the texture so we don't garbage collect and flush
        #the associated texture from the graphics server
        self._tex  = texture 

        log.write( log.DEBUG, "Acquired framebuffer:{0}".format(self._fb))

    def __del__(self):
        hwgfx.framebuffer_drop(self._fb)
        log.write( log.DEBUG, "Dropped framebuffer:{0}".format(self._fb))
        
    @classmethod
    def from_texture(cls,texture):
        fb = hwgfx.framebuffer_create()
        hwgfx.framebuffer_bind_texture( fb, texture._tex )
        return cls(fb,texture)

class framebuffer_as_render_target:
    def __init__(self,framebuffer):
        self.framebuffer = framebuffer

    def __enter__(self):
        framebuffer.stack.append(self.framebuffer)
        hwgfx.framebuffer_render_start( self.framebuffer._fb )

    def __exit__(self, exc_type, exc_value, traceback):
        if(len(framebuffer.stack)>0):
            framebuffer.stack.pop()
            if(len(framebuffer.stack)>0):
                hwgfx.framebuffer_render_start( framebuffer.stack[-1]._fb )
            else:
                hwgfx.framebuffer_render_end( self.framebuffer._fb )

