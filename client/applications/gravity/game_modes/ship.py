from client.ctt2.assets import assets

from .entities.chamber import chamber
from .entities.ow_enviro import ow_enviro
from .entities.ow_player import ow_player
from .entities.ow_terminal import ow_terminal
from .entities.poster import poster
from .entities.ow_planet import ow_planet

class ship_game:
    def __init__(self):
        assets.load_package("intro")
        assets.load_package("station")
        assets.load_package("sylab")
        self.ow_enviro = ow_enviro( assets.get("sylab/dict/landing_enviro_config") )
        self.star_textures = [ assets.get("station/texture/stars_0"),
                               assets.get("station/texture/stars_1"),
                               assets.get("station/texture/stars_2")]

        self.view = assets.get("common/coordsys/16:9")
        self.ow_player = ow_player(self.view)
        self.ow_enviro.register_child( self.ow_player )

        self.lights_on = True
        self.floor_texture = assets.get("station/texture/floor")
        self.star_shader = assets.get("station/shader/star_scroll")
        self.comp_shader = assets.get("common/shader/passthru_filtered")
        self.floor_shader = assets.get("station/shader/floor")
        self.ship_texture = assets.get("intro/texture/ascend_ship")
        self.primitive = assets.get("core/primitive/unit_uv_square")
        self.ow_planet = ow_planet(self.view, self.ow_player, self.ow_enviro )

        self.t_delta = 1.0/240.0;

        self.comp_buffer = assets.exec("core/factory/framebuffer/from_screen")


        self.t = 0
        self.sequencer   = assets.get("station/curve_sequence/station")

        self.chamber = chamber(self.primitive,self.view)

        self.posters = [ 
                            poster( self.view, "unity", self.ow_player, [32.0,0.0], 12, False ) ,
                            poster( self.view, "unity", self.ow_player, [18.0,0.0], 12, False ) ,
                            poster( self.view, "binary", self.ow_player, [-18.0,0.0], 12, True ) ,
                            poster( self.view, "binary", self.ow_player, [-32.0,0.0], 12, False ) 
                       ]

        self.ow_terminal = ow_terminal(self.view, self.ow_player, self.posters, self.comp_buffer.get_texture() )
        self.sequencer.register_slaves([ self.ow_planet, self.ow_player, self.ow_terminal, self.ow_enviro])
        self.sequencer.register_slaves( self.posters )

        self.sequencer.tick()
        self.sequencer.seek_forward(assets.get("sylab/dict/debug_vars")["fw_seek"])


    def tick(self,context):
        self.t+=self.t_delta
        self.sequencer.tick()


    def render_ship(self):
        with assets.get("core/hwgfx/blendmode/alpha_over"):
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
                 "px"                   : self.ow_player.uw_x,
                 "uv_translate"         : [0,0]
            } )

    def render_starscroll(self, paras = [0.01,0.05], triple=True):
        assets.exec("core/hwgfx/context/clear[r,g,b,a]", [0.0,0.0,0.0,1.0])
        with assets.get("core/hwgfx/blendmode/add"):
            self.primitive.render_shaded( self.star_shader, 
                        { "texBuffer"    : self.star_textures[0],
                          "uv_translate" : [self.t*paras[0]+self.ow_player.uw_x*paras[0],0.0],
                          "uv_scale"     : [1,1],
                          "t"            : self.t,
                          "filterColor"  : [0.97,0.97,1.0,1.0]} )
            self.primitive.render_shaded( self.star_shader, 
                        { "texBuffer"    : self.star_textures[1],
                          "uv_translate" : [self.t*paras[1]+self.ow_player.uw_x*paras[1],0.0],
                          "uv_scale"     : [0.5,0.5],
                          "t"            : self.t,
                          "filterColor"  : [0.7,0.65,0.65,0.65]} )

            if triple:
                self.primitive.render_shaded( self.star_shader, 
                            { "texBuffer"    : self.star_textures[2],
                              "uv_translate" : [self.t*0.5+self.ow_player.uw_x*0.03,0.0],
                              "uv_scale"     : [0.2,0.2],
                              "t"            : self.t,
                              "filterColor"  : [0.28,0.35,0.25,0.25]} )


    def render(self,context):
        with self.ow_terminal.get_terminal_buffer().as_render_target():
            with assets.get("core/hwgfx/blendmode/alpha_over"):
                self.render_starscroll([0.003,0.01],False)
                self.ow_terminal.render_termapp()


        with self.comp_buffer.as_render_target():
            assets.exec("core/hwgfx/context/clear[r,g,b,a]",[0.0,0.0,0.0,1.0])
            if(self.lights_on):
                self.render_starscroll([0.01,0.01],False)
            else:
                assets.exec("core/hwgfx/context/clear[r,g,b,a]",[0.0,0.0,0.0,1.0])
            with assets.get("core/hwgfx/blendmode/add"):
                self.ow_terminal.render()
                for poster in self.posters:
                    poster.render()

        self.comp_buffer.render_shaded( self.comp_shader, { "filter_color" : self.sequencer.animated_value("star_fadein") } )
        with assets.get("core/hwgfx/blendmode/add"):
            self.render_floor()
        with assets.get("core/hwgfx/blendmode/alpha_over"):
            self.chamber.render_base( self.sequencer.animated_value("chamber_color") )
            if(self.sequencer.animated_value("sequence.time") > 8.0):
                self.ow_player.render()
            self.render_ship()

            self.chamber.render_door( self.sequencer.animated_value("door_color"))
        self.ow_terminal.render_help()

