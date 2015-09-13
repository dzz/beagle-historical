from client.ctt2.assets import assets
from client.gfx.context import gfx_context
from client.gfx.primitive import primitive
from client.gfx.blend            import blendstate,blendmode
from client.ctt2.animation import curve_sequencer
from client.gfx.framebuffer import *


class ow_player:
    def __init__(self, view):
        self.walk_sequencer = curve_sequencer( assets.get("overworld_player/curve_sequence/walk_left") )
        self.x = 0
        self.y = 0
        self.primitive = primitive.get_unit_uv_primitive()
        self.view = view
        self.shader = assets.get("common/shader/default_2d")

    def tick(self):
        self.walk_sequencer.tick()

    def get_shader_params(self):
        return {
            "texBuffer"            : assets.get( self.walk_sequencer.animated_value("texture_asset") ),
            "scale_local"          : [1.0,1.0],
            "translation_local"    : [0,0,0],
            "scale_local"          : [1.2,1.5],
            "translation_world"    : [self.x,self.y],
            "scale_world"          : [1.0,1.0],
            "view"                 : self.view,
            "rotation_local"       : 0.0 ,
            "filter_color"         : [1.0,1.0,1.0,1.0],
            "uv_translate"         : [0,0] }

    def render(self):
        self.primitive.render_shaded(self.shader, self.get_shader_params() )

    
class chamber:
    def __init__(self,primitive, view):
        self.shader = assets.get("common/shader/default_2d")
        self.base_texture = assets.get("station/texture/chamber_base")
        self.door_texture = assets.get("station/texture/chamber_door")
        self.primitive = primitive
        self.view = view

    def get_shader_params(self,texture, color = [1,1,1,1]):
        return {
                 "texBuffer" : texture,
                 "translation_local"    : [0,0,0],
                 "scale_local"          : [1.2,1.5],
                 "translation_world"    : [0.0,1.3],
                 "scale_world"          : [1.0,1.0],
                 "view"                 : self.view,
                 "rotation_local"       : 0.0 ,
                 "filter_color"         : color,
                 "uv_translate"         : [0,0] }

    def render_base(self, color = [1,1,1,1]):
        self.primitive.render_shaded( self.shader, self.get_shader_params( self.base_texture, color ))

    def render_door(self, color = [1,1,1,1]):
        self.primitive.render_shaded( self.shader, self.get_shader_params( self.door_texture, color ))


class ship_game:
    def __init__(self):
        assets.load_packages(["station","intro"])
        self.star_textures = [ assets.get("station/texture/stars_0"),
                               assets.get("station/texture/stars_1"),
                               assets.get("station/texture/stars_2")]

        self.view = assets.get("common/coordsys/16:9")
        self.ow_player = ow_player(self.view)
        self.floor_texture = assets.get("station/texture/floor")
        self.star_shader = assets.get("station/shader/star_scroll")
        self.comp_shader = assets.get("common/shader/passthru_filtered")
        self.floor_shader = assets.get("station/shader/floor")
        self.ship_texture = assets.get("intro/texture/ascend_ship")
        self.primitive = primitive.get_unit_uv_primitive()
        self.t_delta = 1.0/240.0;

        self.comp_buffer = framebuffer.from_dims(256,128,False)

        self.t = 0
        self.sequence   = assets.get("station/curve_sequence/station")
        self.sequencer = curve_sequencer( self.sequence )
        self.chamber = chamber(self.primitive,self.view)

        self.sequencer.register_slave( self.ow_player )

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
        with blendstate(blendmode.alpha_over):
            self.chamber.render_base( self.sequencer.animated_value("chamber_color") )
            self.render_ship()
            self.chamber.render_door( self.sequencer.animated_value("door_color"))
            self.ow_player.render()

