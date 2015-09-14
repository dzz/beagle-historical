from math import sin,cos
from client.ctt2.assets import assets
from client.ctt2.animation import curve_sequencer
from client.gfx.blend            import blendstate,blendmode
from client.math.helpers import lerp_vec
from client.math.curve import curve
from .system import starfield
from client.gfx.framebuffer import *
from client.gfx.primitive import primitive
from client.gfx.context import gfx_context
import client.system.log as log


class intro_game:
        def __init__(self):
            assets.load_package("intro")
            self.starfield = starfield()
            self.fg_texture = assets.get("intro/texture/planet_background")
            self.bg_texture = assets.get("intro/texture/fire_gradient")
            self.atmo_shader = assets.get("intro/shader/atmo")
            self.star_texture = assets.get("intro/texture/star_background")
            self.star_shader = assets.get("intro/shader/starbg_shader")
            self.comp_shader = assets.get("common/shader/passthru_filtered")
            self.fg_shader  = assets.get("common/shader/default_2d")
            self.ship_shader = assets.get("common/shader/default_2d")
            self.bg_shader  = assets.get("intro/shader/sundistort")
            self.nrg_shader = assets.get("intro/shader/energy")
            self.nrg_texture = assets.get("intro/texture/green_energy")
            self.view       = assets.get("common/coordsys/unit_square")
            self.sequence   = assets.get("intro/curve_sequence/intro")
            self.primitive  = primitive.get_unit_uv_primitive()
             

            self.comp_buffer = framebuffer.from_screen()
            self.star_buffer = framebuffer.from_dims(512,512,True)

            self.sequencer = curve_sequencer(assets.get("intro/curve_sequence/intro"), { "city_launch" : self.render_city_launch, 
                                                                             "ship_atmo"   : self.render_ship_atmo,
                                                                             "threat" : self.render_threat})

        def tick(self, context):
            self.starfield.tick()
            self.sequencer.tick()
            if self.sequencer.finished:
                context.trigger_mode_switch("ship")
                return

        def get_bg_shader_params(self):
            return { 
                     "texBuffer"            : self.bg_texture,
                     "texDist"              : self.star_buffer.get_texture(),
                     "translation_local"    : self.sequencer.animated_value("bg_ease"),
                     "scale_local"          : self.sequencer.animated_value("bg_scale"),
                     "translation_world"    : [0.0,0.0],
                     "scale_world"          : [1,1],
                     "view"                 : self.view,
                     "rotation_local"       : 0.0 ,
                     "filter_color"         : self.sequencer.animated_value("background_fade") ,
                     "uv_translate"         : self.sequencer.animated_value("bg_uv_translate") 
                     } 

        def get_threat_bg_params(self):
            return { 
                     "texBuffer"            : assets.get("intro/texture/threat_bg"),
                     "translation_local"    : [0.0,0.0],
                     "scale_local"          : self.sequencer.animated_value("bg_grad_scale"),
                     "translation_world"    : [0.0,0.0],
                     "scale_world"          : [1,1],
                     "view"                 : assets.get("common/coordsys/unit_square"),
                     "rotation_local"       : self.sequencer.animated_value("sequence.time")*0.001,
                     "filter_color"         : [1.0,1.0,1.0,1.0],
                     "uv_translate"         : [0.0,0.0]
                     } 

        def get_threat_planet_params(self):
            return { 
                     "texBuffer"            : assets.get( self.sequencer.animated_value("planet_texture_asset") ),
                     "translation_local"    : [0.0,0.0],
                     "scale_local"          : [4.0,4.0],
                     "translation_world"    : self.sequencer.animated_value("planet_sprite_path"),
                     "scale_world"          : [1,1],
                     "view"                 : assets.get("common/coordsys/16:9"),
                     "rotation_local"       : 0.0 ,
                     "filter_color"         : [1.0,1.0,1.0,1.0],
                     "uv_translate"         : [0.0,0.0]
                     } 

        def get_cdrom_params(self):
            return { 
                     "texBuffer"            : assets.get( "intro/texture/cdrom" ),
                     "translation_local"    : [0.0,0.0],
                     "scale_local"          : [0.5,0.5],
                     "rotation_local"       : self.sequencer.animated_value("sequence.time")*0.1,
                     "translation_world"    : self.sequencer.animated_value("cdrom_path"),
                     "scale_world"          : [1,1],
                     "view"                 : assets.get("common/coordsys/16:9"),
                     "filter_color"         : [1.0,1.0,1.0,1.0],
                     "uv_translate"         : [0.0,0.0]
                     } 

        def get_fg_shader_params(self):
            return { 
                    "texBuffer"         : self.fg_texture,
                    "translation_local" : self.sequencer.animated_value("planet_ease"),
                   "scale_local"        : self.sequencer.animated_value("planet_scale"),
                   "translation_world"  : [0,0],
                   "scale_world"        : [ 1.3,1.3],
                   "view"               : self.view,
                   "rotation_local"     : [0.0],
                   "filter_color"       : self.sequencer.animated_value("foreground_fade") 
                   } 

        def get_ship_shader_params(self):
            return { 
                    "texBuffer"         : assets.get( self.sequencer.animated_value("ship_texture_asset") ),
                    "translation_local" : [-0.5,-0.5],
                   "scale_local"        : self.sequencer.animated_value("ship_scale"), #[0.024,0.07],
                   "translation_world"  : self.sequencer.animated_value("ship_path"),
                   "scale_world"        : [ 1,1],
                   "view"               : self.view,
                   "rotation_local"     : [0.0],
                   "filter_color"       : self.sequencer.animated_value("ship_color") 
                   } 

        def get_star_shader_params(self):
            return { 
                    "texBuffer"         : self.star_texture,
                    "translation_local" : [0.0,0.0],
                   "scale_local"        : [1.0,1.0],
                   "translation_world"  : [0.0,0.0],
                   "scale_world"        : [ 1,1],
                   "view"               : self.view,
                   "rotation_local"     : [0.0],
                   "filter_color"       : self.sequencer.animated_value("starbg_color"),
                   "uv_translate"       : self.sequencer.animated_value("bg_uv_translate"),
                   "uv_scale"           : self.sequencer.animated_value("bg_uv_scale")
                   } 

        def get_energy_shader_params(self):
            return { 
                    "texBuffer"         : self.nrg_texture,
                    "translation_local" : [0.0,0.0],
                   "scale_local"        : self.sequencer.animated_value("energy_scale"),#[0.01,1.0],
                   "translation_world"  : self.sequencer.animated_value("energy_location"),
                   "scale_world"        : [ 1,1],
                   "view"               : self.view,
                   "rotation_local"     : [0.0],
                   "filter_color"       : self.sequencer.animated_value("energy_color"),
                   "freq"       : self.sequencer.animated_value("energy_freq")
                   } 


        def get_atmo_shader_params(self):
            return {
                        "texBuffer" : self.star_buffer.get_texture(),
                        "modBuffer" : self.bg_texture,
                        "color_top"   : self.sequencer.animated_value("atmo_col_top"),
                        "color_bot"   : self.sequencer.animated_value("atmo_col_bot"),
                        "t" : self.sequencer.animated_value("sequence.time")
                    }

        def render_ship_atmo(self):
            #gfx_context.clear([1.0,1.0,1.0,1.0])
            with render_target(self.star_buffer):
                self.starfield.render()

            
            with blendstate(blendmode.alpha_over):
                self.primitive.render_shaded( self.fg_shader, self.get_threat_planet_params() )
                self.primitive.render_shaded( self.star_shader, self.get_star_shader_params() )
                self.primitive.render_shaded( self.atmo_shader, self.get_atmo_shader_params() )
                with blendstate(blendmode.add):
                        self.primitive.render_shaded( self.nrg_shader, self.get_energy_shader_params() )
                self.primitive.render_shaded( self.ship_shader, self.get_ship_shader_params() )

        def render_threat(self):
            self.primitive.render_shaded( self.fg_shader, self.get_threat_bg_params() )
            with blendstate(blendmode.alpha_over):
                with blendstate(blendmode.add):
                    self.primitive.render_shaded( self.star_shader, self.get_star_shader_params() )
                self.primitive.render_shaded( self.fg_shader, self.get_threat_planet_params() )
                self.primitive.render_shaded( self.fg_shader, self.get_cdrom_params() )

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
            
            self.comp_buffer.render_shaded( self.comp_shader, { "filter_color" : self.sequencer.animated_value("composite_tint") })

        def render(self, context):
            self.sequencer.render()
