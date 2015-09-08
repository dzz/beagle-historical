from client.ctt2.assets import assets
from client.gfx.context import gfx_context
from client.gfx.primitive import primitive
from client.gfx.blend            import blendstate,blendmode
from client.ctt2.animation import curve_sequencer
from client.gfx.framebuffer import *

class ship_game:
    def __init__(self):
        assets.load_packages(["station","intro"])
        self.star_textures = [ assets.get("station/texture/stars_0"),
                               assets.get("station/texture/stars_1"),
                               assets.get("station/texture/stars_2")]

        self.view = assets.get("common/coordsys/16:9")
        self.floor_texture = assets.get("station/texture/floor")
        self.star_shader = assets.get("station/shader/star_scroll")
        self.comp_shader = assets.get("common/shader/passthru_filtered")
        self.floor_shader = assets.get("station/shader/floor")
        self.ship_texture = assets.get("intro/texture/ascend_ship")
        self.primitive = primitive.get_unit_uv_primitive()
        self.t_delta = 1.0/240.0;
        self.comp_buffer = framebuffer.from_screen()
        self.t = 0
        self.sequence   = assets.get("station/curve_sequence/station")
        self.sequencer = curve_sequencer( self.sequence )

    def tick(self,context):
        self.t+=self.t_delta
        self.sequencer.tick()

    def render_ship(self):
        with blendstate(blendmode.alpha_over):
            self.primitive.render_shaded( assets.get("common/shader/default_2d"), { 
                    "texBuffer" : self.ship_texture,
                     "translation_local"    : [0.0,0],
                     "scale_local"          : [0.48,0.84],
                     "translation_world"    : self.sequencer.animated_value("ship_path"),
                     "scale_world"          : self.sequencer.animated_value("ship_scale"),
                     "view"                 : self.view,
                     "rotation_local"       : 0.0 ,
                     "filter_color"         : self.sequencer.animated_value("ship_color"),
                     "uv_translate"         : [0,0] })

    def render_floor(self):
        self.primitive.render_shaded( self.floor_shader, { 
                "texBuffer" : self.floor_texture,
                "blrBuffer" : self.comp_buffer.get_texture(),
                 "translation_local"    : [0.0,0],
                 "scale_local"          : self.sequencer.animated_value("floor_dims"),
                 "translation_world"    : self.sequencer.animated_value("floor_path"),
                 "scale_world"          : [1.0,1.0],
                 "view"                 : self.view,
                 "rotation_local"       : 0.0 ,
                 "filter_color"         : [1.0,1.0,1.0,1.0],
                 "uv_translate"         : [0,0]
            } )

    def render_starscroll(self):
        gfx_context.clear([0.0,0.0,0.0,1.0])
        with blendstate(blendmode.add):
            self.primitive.render_shaded( self.star_shader, 
                        { "texBuffer"    : self.star_textures[0],
                          "uv_translate" : [self.t*0.01,0.0],
                          "uv_scale"     : [1,1],
                          "t"            : self.t,
                          "filterColor"  : [0.97,0.97,1.0,1.0]} )
            self.primitive.render_shaded( self.star_shader, 
                        { "texBuffer"    : self.star_textures[1],
                          "uv_translate" : [self.t*0.05,0.0],
                          "uv_scale"     : [0.5,0.5],
                          "t"            : self.t,
                          "filterColor"  : [0.7,0.65,0.65,0.65]} )
            self.primitive.render_shaded( self.star_shader, 
                        { "texBuffer"    : self.star_textures[2],
                          "uv_translate" : [self.t*0.5,0.0],
                          "uv_scale"     : [0.2,0.2],
                          "t"            : self.t,
                          "filterColor"  : [0.28,0.35,0.25,0.25]} )


    def render(self,context):
        with render_target(self.comp_buffer):
            self.render_starscroll()
        self.comp_buffer.render_shaded( self.comp_shader, { "filter_color" : self.sequencer.animated_value("star_fadein") } )
        self.render_floor()
        self.render_ship()

