from math import sin,cos
from client.ctt2.assets import assets
from client.gfx.blend            import blendstate,blendmode
from client.math.helpers import lerp_vec
from client.math.curve import curve
from .system import starfield
from client.gfx.framebuffer import *
from client.gfx.primitive import primitive


class intro_game:
        def __init__(self):
            self.starfield = starfield()
            self.fg_texture = assets.get("intro/texture/planet_background")
            self.bg_texture = assets.get("intro/texture/fire_gradient")
            self.fg_shader  = assets.get("common/shader/default_2d")
            self.bg_shader  = assets.get("intro/shader/sundistort")
            self.view       = assets.get("common/coordsys/unit_square")
            self.config     = assets.get("intro/dict/sunrise_config")
            self.primitive  = primitive.get_unit_uv_primitive()
             

            self.comp_buffer = framebuffer.from_dims(960,540,False)
            self.star_buffer = framebuffer.from_dims(256,256,False)
            self.curves = {}

            for key in self.config["curves"]:
                self.curves[key] = curve(self.config["curves"][key])

            self.t          = 0.0

        def tick(self, context):
            self.t += 1.0/60.0
            self.starfield.tick()

        def get_lerped(self,key):
            r = self.curves[key].value_at(self.t)
            return r

        def get_bg_shader_params(self):
            return { 
                     "texBuffer"            : self.bg_texture,
                     "texDist"              : self.star_buffer.get_texture(),
                     "translation_local"    : self.get_lerped("planet_ease"),
                     "scale_local"          : [1,1],
                     "translation_world"    : self.get_lerped("planet_ease"),
                     "scale_world"          : [1,1],
                     "view"                 : self.view,
                     "rotation_local"       : 0.0 ,
                     "filter_color" : self.get_lerped("background_fade") 
                     } 

        def render(self, context):

            with render_target(self.star_buffer):
                with blendstate(blendmode.alpha_over):
                    self.starfield.render()

            with render_target(self.comp_buffer):
                self.primitive.render_shaded( self.bg_shader, self.get_bg_shader_params() )

                with blendstate(blendmode.alpha_over):
                    self.fg_texture.render_processed( self.fg_shader, {
                                                                    "translation_local" : self.get_lerped("planet_ease"),
                                                                    "scale_local" : [1,1],
                                                                    "translation_world": [0,0],
                                                                    "scale_world" : [ 1,1],
                                                                    "view"        : self.view,
                                                                    "rotation_local" : [0.0],
                                                                    "filter_color" : self.get_lerped("foreground_fade") } )


            
            #test_shdr = shaders.get_client_program("no_transform","postfx/intro/sundistort")
            #self.primitive.render_shaded( test_shdr, { "buffer" : self.star_buffer.get_texture(), "dist" : self.comp_buffer.get_texture() } );
            self.comp_buffer.render_processed( shaders.get_client_program("no_transform","postfx/passthru") )

