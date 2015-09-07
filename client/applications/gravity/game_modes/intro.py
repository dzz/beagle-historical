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


            assets.load_package("intro")
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
             

            self.comp_buffer = framebuffer.from_screen()
            self.star_buffer = framebuffer.from_dims(512,512,True)
            self.curves = {}

            for sceneKey in self.config["curves"]:
                self.curves[sceneKey] = {}
                for curveKey in self.config["curves"][sceneKey]:
                    self.curves[sceneKey][curveKey] = curve(self.config["curves"][sceneKey][curveKey])

            self.t                   = 0.0
            self.total_t             = 0.0
            self.delta_t             = 1.0/60.0 * self.config["timescale"]
            self.scene_renderers     = {
                                            "city_launch" : self.render_city_launch,
                                            "ship_dock"   : self.render_ship_dock }
            self.current_scene_renderer = None
            self.current_scene_key      = None

        def tick(self, context):
            self.total_t += self.delta_t

            next_scene_renderer = None
            for sceneKey in self.config["scenes"]:
                sceneDef = self.config["scenes"][sceneKey]
                if self.total_t > sceneDef["start"] and self.total_t < sceneDef["end"]:
                    self.t = self.total_t - sceneDef["start"]
                    next_scene_renderer = self.scene_renderers[sceneKey]
                    self.current_scene_key = sceneKey
                    break

            self.current_scene_renderer = next_scene_renderer

            self.starfield.tick()

        def get_lerped(self,key):
            if(self.current_scene_key):
                r = self.curves[self.current_scene_key][key].value_at(self.t)
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
                   "scale_local"        : self.get_lerped("planet_scale"),
                   "translation_world"  : [0,0],
                   "scale_world"        : [ 1.3,1.3],
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
                   "translation_world"  : [-0.483,0],
                   "scale_world"        : [ 1,1],
                   "view"               : self.view,
                   "rotation_local"     : [0.0],
                   "filter_color"       : self.get_lerped("energy_color"),
                   "freq"       : self.get_lerped("energy_freq")
                   } 


        def render_ship_dock(self):
            gfx_context.clear([1.0,1.0,1.0,1.0])

        def render_city_launch(self):
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
            
            self.comp_buffer.render_processed( shaders.get_client_program("no_transform","postfx/passthru_filter"),
                        {
                            "filter_color" : self.get_lerped("composite_tint") 
                        })

        def render(self, context):
            if(self.t > self.config["ending"]):
                gfx_context.clear([0.0,0.0,0.0,1.0])
                return
            else:
                self.current_scene_renderer()


