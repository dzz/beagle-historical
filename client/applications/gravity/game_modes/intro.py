from math import sin,cos
from client.ctt2.assets import assets
from client.gfx.blend            import blendstate,blendmode
from client.math.helpers import lerp_vec
from client.math.curve import curve
from .system import starfield
from client.gfx.framebuffer import *
from client.gfx.primitive import primitive
from client.gfx.context import gfx_context


class intro_game:
        def __init__(self):
            self.starfield = starfield()
            self.fg_texture = assets.get("intro/texture/planet_background")
            self.bg_texture = assets.get("intro/texture/fire_gradient")
            self.fg_shader  = assets.get("common/shader/default_2d")
            self.ship_shader = assets.get("common/shader/default_2d")
            self.bg_shader  = assets.get("intro/shader/sundistort")
            self.nrg_shader = assets.get("intro/shader/energy")
            self.nrg_texture = assets.get("intro/texture/green_energy")
            self.ship_texture = assets.get("intro/texture/ascend_ship")
            self.view       = assets.get("common/coordsys/unit_square")
            self.config     = assets.get("intro/dict/sunrise_config")
            self.primitive  = primitive.get_unit_uv_primitive()
             

            self.comp_buffer = framebuffer.from_dims(960,540,False)
            self.star_buffer = framebuffer.from_dims(960,540,True)
            self.curves = {}

            for key in self.config["curves"]:
                self.curves[key] = curve(self.config["curves"][key])

            self.t          = 0.0

        def tick(self, context):
            self.t += 1.0/60.0 * self.config["timescale"]
            self.starfield.tick()

        def get_lerped(self,key):
            r = self.curves[key].value_at(self.t)
            return r

        def get_bg_shader_params(self):
            return { 
                     "texBuffer"            : self.bg_texture,
                     "texDist"              : self.star_buffer.get_texture(),
                     "translation_local"    : self.get_lerped("bg_ease"),
                     "scale_local"          : self.get_lerped("bg_scale"),
                     "translation_world"    : [0.0,0.0],
                     "scale_world"          : [1,1],
                     "view"                 : self.view,
                     "rotation_local"       : 0.0 ,
                     "filter_color"         : self.get_lerped("background_fade") ,
                     "uv_translate"         : self.get_lerped("bg_uv_translate") 
                     } 

        def get_fg_shader_params(self):
            return { 
                    "texBuffer"         : self.fg_texture,
                    "translation_local" : self.get_lerped("planet_ease"),
                   "scale_local"        : [1,1],
                   "translation_world"  : [0,0],
                   "scale_world"        : [ 1,1],
                   "view"               : self.view,
                   "rotation_local"     : [0.0],
                   "filter_color"       : self.get_lerped("foreground_fade") 
                   } 

        def get_ship_shader_params(self):
            return { 
                    "texBuffer"         : self.ship_texture,
                    "translation_local" : [-0.5,-0.5],
                   "scale_local"        : self.get_lerped("ship_scale"), #[0.024,0.07],
                   "translation_world"  : self.get_lerped("ship_path"),
                   "scale_world"        : [ 1,1],
                   "view"               : self.view,
                   "rotation_local"     : [0.0],
                   "filter_color"       : self.get_lerped("foreground_fade") 
                   } 

        def get_energy_shader_params(self):
            return { 
                    "texBuffer"         : self.nrg_texture,
                    "translation_local" : [0.0,0.0],
                   "scale_local"        : self.get_lerped("energy_scale"),#[0.01,1.0],
                   "translation_world"  : [-0.283,0],
                   "scale_world"        : [ 1,1],
                   "view"               : self.view,
                   "rotation_local"     : [0.0],
                   "filter_color"       : self.get_lerped("energy_color"),
                   "freq"       : self.get_lerped("energy_freq")
                   } 

        def render(self, context):

            if(self.t > self.config["ending"]):
                gfx_context.clear([0.0,0.0,0.0,1.0])
                return

            with render_target(self.star_buffer):
                with blendstate(blendmode.alpha_over):
                    self.starfield.render()

            with render_target(self.comp_buffer):
                self.primitive.render_shaded( self.bg_shader, self.get_bg_shader_params() )
                with blendstate(blendmode.add):
                    self.primitive.render_shaded( self.nrg_shader, self.get_energy_shader_params() )

                with blendstate(blendmode.alpha_over):
                    self.primitive.render_shaded( self.fg_shader, self.get_fg_shader_params() )
                with blendstate(blendmode.alpha_over):
                    self.primitive.render_shaded( self.ship_shader, self.get_ship_shader_params() )



            
            #test_shdr = shaders.get_client_program("no_transform","postfx/intro/sundistort")
            #self.primitive.render_shaded( test_shdr, { "buffer" : self.star_buffer.get_texture(), "dist" : self.comp_buffer.get_texture() } );
            self.comp_buffer.render_processed( shaders.get_client_program("no_transform","postfx/passthru_filter"),
                        {
                            "filter_color" : self.get_lerped("composite_tint") 
                        })

