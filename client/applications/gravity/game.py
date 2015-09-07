import hwgfx
from client.gfx import context
from math import floor
from client.system.video import *
import client.system.log as log
import client.gfx.shaders as shaders
from client.gfx.primitive import primitive, draw_mode
from client.math.helpers import tesselated_unit_quad, tesselated_unit_quad_uv
from .background import background
from .vortex import vortex
from .player import player
from .particle import particle
from .pickup import pickup
from math import sqrt,atan2,cos,sin
from client.system.gamepad       import get_gamepad
from client.gfx.sprite           import sprite, sprite_renderer
from client.gfx.blend            import blendstate,blendmode
from client.gfx.tileset          import tileset
from client.gfx.coordinates      import centered_view, Y_Axis_Down
from random import choice, uniform, sample
from client.math.helpers import distance
from .music_system import music_system
from .game_modes.vortex import vortex_game
from .game_modes.system import system_game
from .game_modes.intro import intro_game
from client.gfx.framebuffer import *
from client.gfx.texture import texture
from client.gfx.text import render_text

import hwgfx
from random import sample


def shuffled_range(start,end):
    r = range(start+1,end+1)
    s = sample(r, len(r))
    return s

class game:
    def level_up(self):
        self.player.points += self.pickup.display_level * self.player.speed * self.pickup.level
        self.pick_renderers()
        self.pick_post_processing_shader()
        for bg in self.backgrounds:
            bg.randomize_colors()
        self.num_render_backgrounds = choice(range(1,self.max_num_render_backgrounds))

    def pick_renderers(self):
        self.sprite_renderer = choice([
            self.sprite_renderers[0],
            self.sprite_renderers[1]])
        self.radar_sprite_renderer = choice(self.sprite_renderers)
        self.part_buffer_sprite_renderer = choice(self.sprite_renderers)
        self.part_sprite_renderer = choice(self.sprite_renderers)

    def create_particle_classes(self):
        particle.create_particle_class("pickup_explosion", choice( self.sprite_renderers ) )
        particle.create_particle_class("comet_tail", choice( self.sprite_renderers ) )
        particle.create_particle_class("sprinkles", choice( self.sprite_renderers ) )

    def load_postfx_shaders(self):
        self.postfx_shader_inputs = []
        self.postfx_shaders =  [ 
                                 shaders.get_client_program( "no_transform","postfx/inversoflux"),
                                 shaders.get_client_program( "no_transform","postfx/passthru"), 
                                 shaders.get_client_program( "no_transform","postfx/mind_meld"), 
                                 shaders.get_client_program( "no_transform","postfx/mult_bufs"),
                                 shaders.get_client_program( "no_transform","postfx/invert"),
                                 shaders.get_client_program( "no_transform","postfx/sin_map"),
                                 shaders.get_client_program( "no_transform","postfx/sin_map_rgb") 
                                 ]

    def pick_post_processing_shader(self):
        self.postfx_shader_inputs = [ 
                                        ("factor_a", [ choice([0.01,0.05,0.1 ])] ),
                                        ("factor_b", [ choice([0.1,0.5 ])] ) 
                                    ]
        self.postfx_shader = choice( self.postfx_shaders )

    def trigger_mode_switch(self,mode):
            self.current_mode = self.modes[mode]

    def __init__(self):
       log.set_level( log.ERROR )
       self.modes = {
                      "intro"  : intro_game(),
                      "system" : system_game(),
                      "vortex" : vortex_game() }

       self.current_mode = self.modes["intro"]

       self.load_postfx_shaders()
       self.pick_post_processing_shader()
       self.music_system = music_system("devon.music")
       self.world_zoom_current = 1.0
       self.jitter_radar_shows = False
       self.t = 0
       self.backgrounds = [ background(), background(), background()  ]
       self.num_render_backgrounds = 1
       self.max_num_render_backgrounds = 3
       self.sprite_tilesets = []
       self.sprite_renderers = []

       self.distortion_buffer = framebuffer.from_dims(64,64, True)
       self.player_overlay = framebuffer.from_dims(640,480, False)
       self.primary_buffer = framebuffer.from_screen()

       self.hud_buffer = framebuffer.from_dims(24*8,12*8)

       configuration_template = { "image": "", "imageheight": 192,"imagewidth": 112, "margin": 0, "spacing": 0, "properties": {}, "firstgid": 0, "tileheight": 16, "tilewidth": 16, "tileproperties" : {} }

       tileset_imgs = [ "set_00.png","set_01.png","set_02.png","set_03.png","set_04.png" ]

       for img in tileset_imgs:
           configuration_template["image"] = img
           loaded_tiles = tileset( configuration = configuration_template, img_path = "sprite/" )
           self.sprite_renderers.append(sprite_renderer( tileset = loaded_tiles, coordinates = centered_view(1920,1080, Y_Axis_Down)))
           self.sprite_renderers[0]

       self.pick_renderers()

       self.priming_sprite =            sprite( sprite_renderer = self.sprite_renderer, named_animations = { "default" : [11,12] }, current_animation = "default", ticks_per_frame = 3)
       self.reticle_sprite =            sprite( sprite_renderer = self.sprite_renderer, named_animations = { "default" : [10] }, current_animation = "default", ticks_per_frame = 1)
       self.player_sprite =             sprite( sprite_renderer = self.sprite_renderer, named_animations = { "default" : [0] }, current_animation = "default", ticks_per_frame = 1)
       self.alternate_player_sprite =   sprite( sprite_renderer = self.sprite_renderer, named_animations = { "default" : [42,43,44,45] }, current_animation = "default", ticks_per_frame = 4)

       self.active_player_sprite = self.player_sprite

       self.engine_sprite =  sprite( sprite_renderer = self.sprite_renderer, named_animations = { "default" : [1] }, current_animation = "default", ticks_per_frame = 1)
       self.fire_sprite =    sprite( sprite_renderer = self.sprite_renderer, named_animations = { "default" : [2,3] }, current_animation = "default", ticks_per_frame = 5)
       self.emerald_sprite = sprite( sprite_renderer = self.sprite_renderer, named_animations = { "default" : [7,8,9,8] }, current_animation = "default", ticks_per_frame = 9) 

       self.radar_sprites = [ sprite( sprite_renderer = self.sprite_renderer, named_animations = { "default" : shuffled_range( 16, 20) }, current_animation = "default", ticks_per_frame = 9),
                              sprite( sprite_renderer = self.sprite_renderer, named_animations = { "default" : shuffled_range(16,20) }, current_animation = "default", ticks_per_frame = 13),
                              sprite( sprite_renderer = self.sprite_renderer, named_animations = { "default" : range(42,47) }, current_animation = "default", ticks_per_frame = 20),
                              sprite( sprite_renderer = self.sprite_renderer, named_animations = { "default" : range(42,47) }, current_animation = "default", ticks_per_frame = 17) ]


       self.create_particle_classes()
       self.music_system.play()
       self.vortex = vortex()
       self.player = player()
       self.pickup = pickup(256,256,self.player,self.vortex,self)
       self.particles = []
                    


    def tick(self):
        self.current_mode.tick(self)

    def render(self):
        self.current_mode.render(self)


