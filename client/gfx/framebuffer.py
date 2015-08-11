import client.system.log as log
import client.gfx.texture as texture
import client.gfx.shaders as shaders
from client.gfx.primitive import primitive, draw_mode
from client.math.helpers import tesselated_unit_quad, tesselated_unit_quad_uv
from client.system.video import *
import hwgfx

class framebuffer:
    stack = []
    screen_primitive = primitive( draw_mode.TRIS, tesselated_unit_quad, tesselated_unit_quad_uv )
    def __init__(self, fb, texture):
        self._fb   = fb
        #we grab a reference to the texture so we don't garbage collect and flush
        #the associated texture from the graphics server
        self._tex  = texture 

        log.write( log.DEBUG, "Acquired framebuffer:{0}".format(self._fb))

    def __del__(self):
        hwgfx.framebuffer_drop(self._fb)
        log.write( log.DEBUG, "Dropped framebuffer:{0}".format(self._fb))

    def bind_as_texture(self,texture_unit):
        self._tex.bind(texture_unit)

    def render_processed( self, shader_program, additional_buffers = [], shader_inputs = [] ):
        self.bind_as_texture( texture.units[0] )
        idx = 1
        for fb in additional_buffers:
            fb.bind_as_texture(idx)
            idx+=1
        shader_program.bind( shader_inputs ) 
        framebuffer.screen_primitive.render()
        
    @classmethod
    def from_texture(cls,texture):
        fb = hwgfx.framebuffer_create()
        hwgfx.framebuffer_bind_texture( fb, texture._tex )
        return cls(fb,texture)

    @classmethod
    def from_screen(cls):
       tex = texture.texture.from_dims(get_screen_width(),get_screen_height(),False)
       return framebuffer.from_texture(tex)

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

class render_target(framebuffer_as_render_target):
    pass
