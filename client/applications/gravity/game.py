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
from client.gfx.framebuffer import *
from client.gfx.texture import texture

import hwgfx
from random import sample

def tick_particles(particles,vortex, t):
    max_particles = 300
    tickset = t%4
    if(len(particles)>max_particles):
        particles = sample(particles,max_particles)
    new_particles = []

    idx = 0
    for part in particles:
        idx+=1
        if(idx%4==tickset):
            part.tick(vortex)
        if(part.ttl>0):
            new_particles.append(part)
    return new_particles

def shuffled_range(start,end):
    r = range(start+1,end+1)
    s = sample(r, len(r))
    return s

class game:
    def level_up(self):
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

    def __init__(self):
       log.set_level( log.ERROR | log.WARNING | log.INFO  )
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

       self.distortion_buffer = framebuffer.from_screen()
       self.primary_buffer = framebuffer.from_screen()

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

       particle.tick_sprites()
       self.t +=1
       pad = get_gamepad(0)
       for sprite in self.radar_sprites:
           sprite.tick()

       if(self.t%choice([3,5,9])==0 and self.player.firing>0):
           self.particles.append( particle( self.player.x - self.player.vx, self.player.y - self.player.vy, 0,0, self.player.r, "sprinkles"))

       if(self.t%choice([2,3,5,7,9])==0):
           # comet tail
           p_vec_x = self.pickup.x - self.player.x
           p_vec_y = self.pickup.y - self.player.y
           r = atan2(p_vec_x,p_vec_y)+uniform(-0.1,0.1)
           dist = distance( self.player.x, self.player.y, self.pickup.x,self.pickup.y )
           d = uniform((dist/12), (dist/220) )
           vx = -1*sin(r)*d
           vy = -1*cos(r)*d
           self.particles.append( particle( self.pickup.x, self.pickup.y, vx,vy, self.player.r, "comet_tail"))

       self.pickup.tick(self.particles, self.sprite_renderer, self.music_system )

       if(self.pickup.levelled):
           self.active_player_sprite = choice([self.player_sprite, self.alternate_player_sprite])
           self.jitter_radar_shows = choice([True,False, False])

       self.particles = tick_particles(self.particles,self.vortex, self.t)
       for bg in self.backgrounds:
           bg.tick(self.vortex.td_current, self.player.r)
           bg.x = self.player.x
           bg.y = self.player.y

       self.priming_sprite.tick()
       self.emerald_sprite.tick()
       self.music_system.tick()
       self.player.tick(pad,self.vortex,self.music_system)
       self.fire_sprite.tick()

       self.alternate_player_sprite.tick()

       a=0.98
       b=0.02
       self.world_zoom_current = (self.world_zoom_current*a)+(pad.triggers[0]*b)

    def render(self):
        wobble = (sin (self.t/12)+1)/2.0
        world_zoom_min = 0.2
        world_zoom_max = 0.8
        target_x = self.pickup.x-self.player.x
        target_y = self.pickup.y-self.player.y
        dist = sqrt(target_x*target_x+target_y*target_y)
        reticle_r = 3.14-atan2(target_x,target_y)
        world_zoom = world_zoom_max -(self.world_zoom_current*(world_zoom_max-world_zoom_min))

        # set up some batches 
        primary_batch       = [];
        distortion_batch    = []
        particle_batch      = []

        if(dist>10):
            #add our 'radar' sprites to the distortion batch
            (x1,y1,x2,y2) = (self.player.x,self.player.y,self.pickup.x,self.pickup.y)
            normalize_factor = 1.0 / (max(1,distance(x1,y1,x2,y2)) / 350)
            (x1,y1,x2,y2) = (x1*normalize_factor,y1*normalize_factor,x2*normalize_factor,y2*normalize_factor)

            radar_world_zoom = 1.0
            
            if self.pickup.radars_wobble:
                radar_world_zoom = wobble

            distortion_batch.append([self.radar_sprites[0], [0,-8], 12+wobble*2, -wobble, [x1,y1], radar_world_zoom ])
            distortion_batch.append([self.radar_sprites[1], [-8,-8], 8+wobble*4, wobble, [x2,y2], radar_world_zoom ])
            distortion_batch.append([self.alternate_player_sprite, [-8,-8], 20, reticle_r, [x1,y1], -1*radar_world_zoom ])

            if( self.jitter_radar_shows):
                distortion_batch.append([self.radar_sprites[ choice([0,0,0,0,0,1,1,1,2,2,3])], [0,-8], -50, 0-self.player.r, [x2,y2], -1*radar_world_zoom ])

        # add particles to a batch
        for part in self.particles:
            particle_batch.append([ part.sprite, [-8,-8], 4+(part.r*2), part.r, [part.x-self.player.x,part.y-self.player.y], world_zoom ]) 

        # add the target to the primary batch
        primary_batch.append([self.emerald_sprite, [-8,-8], (3+(wobble*3))*2, atan2(self.pickup.x,self.pickup.y), [self.pickup.x-self.player.x,self.pickup.y-self.player.y], world_zoom ])

        # add some state dependent sprites to the primary batch
        if(self.player.firing>0):
            if(self.player.real_acc>0.98):
                primary_batch.append([ self.fire_sprite, [-8,-2+self.player.acc*3], 6 + self.player.real_acc, self.player.eng_r, [0.0,0.0], world_zoom ])
            else:
                primary_batch.append([ self.priming_sprite, [-8,-6+self.player.acc], 16, self.player.eng_r, [0.0,0.0], world_zoom ])
            primary_batch.append([ self.engine_sprite, [-8,-8+self.player.acc], 4, self.player.eng_r, [0.0,0.0], world_zoom ])

        # add the player to the primary batch
        primary_batch.append([   self.active_player_sprite, [-8,-8], 10+(wobble*3)-(self.player.firing*2), self.player.r, [0.0,0.0], world_zoom ])

        # add the reticle to the primary batch
        if(dist>100):
            primary_batch.append([   self.reticle_sprite, [-8,-32], 4, reticle_r, [0,0], world_zoom ])

        ############################################
        # now we actually blast out the pixels...  #
        ############################################



        self.primary_buffer.bind_as_texture(texture.units[0])
        with render_target(self.distortion_buffer):
            with blendstate(blendmode.alpha_over):
                self.backgrounds[0].render(world_zoom)
                self.part_sprite_renderer.render(particle_batch)
            with blendstate(blendmode.add):
                self.radar_sprite_renderer.render(distortion_batch)
                
        with render_target(self.primary_buffer):
            with blendstate(blendmode.alpha_over):
                self.distortion_buffer.bind_as_texture(texture.units[0])
                for bg in self.backgrounds:
                    bg.render(world_zoom)

            with blendstate(self.pickup.particle_blend_mode):
                self.part_sprite_renderer.render(particle_batch)

            with blendstate(blendmode.alpha_over):
                self.sprite_renderer.render(primary_batch)
                with render_target(self.distortion_buffer ):
                    with blendstate(blendmode.darken):
                        self.sprite_renderer.render(primary_batch)

        self.primary_buffer.render_processed( self.postfx_shader, additional_buffers = [ self.distortion_buffer ],
                shader_inputs = (self.postfx_shader_inputs + [ ("vx", [ self.player.vx ]), ("vy", [self.player.vy]) ]) ) 

