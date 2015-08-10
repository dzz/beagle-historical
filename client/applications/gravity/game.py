import client.system.log as log
from .background import background
from .vortex import vortex
from .player import player
from .particle import particle
from .pickup import pickup
from math import sqrt,atan2,cos,sin
from client.system.gamepad       import get_gamepad
from client.gfx.sprite           import sprite, sprite_renderer
import client.gfx.blend as blend
from client.gfx.tileset          import tileset
from client.gfx.coordinates      import centered_view, Y_Axis_Down
from random import choice, uniform, sample
from client.math.helpers import distance
from .music_system import music_system
from client.gfx.framebuffer import *
from client.gfx.texture import texture

import hwgfx
from random import sample

def tick_particles(particles,vortex):
    max_particles = 200
    if(len(particles)>max_particles):
        particles = sample(particles,max_particles)
    new_particles = []
    for part in particles:
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
        self.background2.randomize_colors()

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

    def __init__(self):
       log.set_level( log.ERROR | log.WARNING | log.INFO | log.DEBUG | log.GFX_MSG)
       self.music_system = music_system("devon.music")
       self.world_zoom_current = 1.0
       self.jitter_radar_shows = False
       self.t = 0
       self.background = background()
       self.background2 = background()
       self.radar_texture = texture.from_dims(1024,1024, False)
       self.radar_buffer = framebuffer.from_texture( self.radar_texture )
       self.sprite_tilesets = []
       self.sprite_renderers = []

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

       self.engine_sprite =             sprite( sprite_renderer = self.sprite_renderer, named_animations = { "default" : [1] }, current_animation = "default", ticks_per_frame = 1)
       self.fire_sprite =               sprite( sprite_renderer = self.sprite_renderer, named_animations = { "default" : [2,3] }, current_animation = "default", ticks_per_frame = 5)
       self.emerald_sprite =            sprite( sprite_renderer = self.sprite_renderer, named_animations = { "default" : [7,8,9,8] }, current_animation = "default", ticks_per_frame = 9) 

       self.radar_sprites = [ 
                                sprite( sprite_renderer = self.sprite_renderer, named_animations = { "default" : shuffled_range( 16, 20) }, current_animation = "default", ticks_per_frame = 9),
                                sprite( sprite_renderer = self.sprite_renderer, named_animations = { "default" : shuffled_range(16,20) }, current_animation = "default", ticks_per_frame = 13),
                                sprite( sprite_renderer = self.sprite_renderer, named_animations = { "default" : range(42,47) }, current_animation = "default", ticks_per_frame = 20),
                                sprite( sprite_renderer = self.sprite_renderer, named_animations = { "default" : range(42,47) }, current_animation = "default", ticks_per_frame = 17) ]


       self.create_particle_classes()
       self.music_system.play()
       self.vortex = vortex()
       self.player = player()
       self.pickup = pickup(16,16,self.player,self.vortex,self)
       self.particles = []
                    

    def tick(self):
       particle.tick_sprites()
       self.t +=1
       pad = get_gamepad(0)
       for sprite in self.radar_sprites:
           sprite.tick()
       if(self.t%choice([3,5,9])==0 and self.player.firing>0):
           self.particles.append( particle( 
                    self.player.x - self.player.vx,
                    self.player.y - self.player.vy,
                    0,0,
                    self.player.r,
                    "sprinkles"
               ))

       if(self.t%choice([1,1,3,5,7,9])==0):
           # comet tail
           p_vec_x = self.pickup.x - self.player.x
           p_vec_y = self.pickup.y - self.player.y

           r = atan2(p_vec_x,p_vec_y)+uniform(-0.1,0.1)

           dist = distance( self.player.x, self.player.y, self.pickup.x,self.pickup.y )
           d = uniform((dist/12), (dist/220) )
           vx = -1*sin(r)*d
           vy = -1*cos(r)*d


           self.particles.append( particle( 
                    self.pickup.x,
                    self.pickup.y,
                    vx,vy,
                    self.player.r, 
                    "comet_tail"
               ))

       self.pickup.tick(self.particles, self.sprite_renderer, self.background, self.music_system )

       if(self.pickup.levelled):
           self.active_player_sprite = choice([self.player_sprite, self.alternate_player_sprite])
           self.jitter_radar_shows = choice([True,False, False])

       self.particles = tick_particles(self.particles,self.vortex)
       self.background.update(self.vortex.td_current, self.player.r)
       self.background2.update(self.vortex.td_current, self.player.r)
       self.priming_sprite.tick()
       self.emerald_sprite.tick()
       self.music_system.tick()
       self.player.tick(pad,self.vortex,self.music_system)
       self.fire_sprite.tick()
       self.background.x = self.player.x
       self.background.y = self.player.y
       self.background2.x = self.player.x*0.24
       self.background2.y = self.player.y*0.24
       self.alternate_player_sprite.tick()

       a=0.98
       b=0.02
       self.world_zoom_current = (self.world_zoom_current*a)+(pad.triggers[0]*b)

    def render(self):
        pad = get_gamepad(0)
        wobble = (sin (self.t/12)+1)/2.0

        world_zoom_min = 0.2
        world_zoom_max = 0.8
        target_x = self.pickup.x-self.player.x
        target_y = self.pickup.y-self.player.y
        dist = sqrt(target_x*target_x+target_y*target_y)
        reticle_r = 3.14-atan2(target_x,target_y)


        world_zoom = world_zoom_max -(self.world_zoom_current*(world_zoom_max-world_zoom_min))
        self.radar_texture.bind(0)
        hwgfx.manual_blend_enter(1)
        self.background2.render(world_zoom*0.5)
        hwgfx.manual_blend_enter(6000)
        self.background.render(world_zoom)
        with framebuffer_as_render_target( self.radar_buffer ):
            self.background.render(world_zoom)
            hwgfx.manual_blend_enter(6000)
            self.background2.render(world_zoom)

        batch  = [];
        shadow_batch = []
        particle_batch = []

        if(dist>10):

            x1 = self.player.x
            y1 = self.player.y
            x2 = self.pickup.x
            y2 = self.pickup.y

            length = max(1,distance(x1,y1,x2,y2)) / 350

            x1/=length
            x2/=length
            y1/=length
            y2/=length


            radar_world_zoom = 1.0
            
            if self.pickup.radars_wobble :
                radar_world_zoom = wobble

            shadow_batch.append([   self.radar_sprites[0], [0,-8], 12+wobble*2, -wobble, [x1,y1], radar_world_zoom ])
            shadow_batch.append([   self.radar_sprites[1], [-8,-8], 8+wobble*4, wobble, [x2,y2], radar_world_zoom ])
            shadow_batch.append([   self.alternate_player_sprite, [-8,-8], 20, reticle_r, [x1,y1], -1*radar_world_zoom ])

            if( self.jitter_radar_shows):
                shadow_batch.append([   self.radar_sprites[ choice([0,0,0,0,0,1,1,1,2,2,3])], [0,-8], -50, 0-self.player.r, [x2,y2], -1*radar_world_zoom ])


        for part in self.particles:
            particle_batch.append([ part.sprite, [-8,-8], 4+(part.r*2), part.r, [part.x-self.player.x,part.y-self.player.y], world_zoom ]) 


        with framebuffer_as_render_target( self.radar_buffer ):
            hwgfx.manual_blend_enter(0)
            self.radar_sprite_renderer.render(shadow_batch)
            hwgfx.manual_blend_enter(5000)
            self.part_buffer_sprite_renderer.render(particle_batch)
        hwgfx.manual_blend_enter(self.pickup.particle_blend_mode)
        self.part_sprite_renderer.render(particle_batch)


        hwgfx.manual_blend_enter(0)

        batch.append([   self.emerald_sprite, [-8,-8], (3+(wobble*3))*2, atan2(self.pickup.x,self.pickup.y), [self.pickup.x-self.player.x,self.pickup.y-self.player.y], world_zoom ])

        if(self.player.firing>0):
            if(self.player.real_acc>0.98):
                batch.append([ self.fire_sprite, [-8,-2+self.player.acc*3], 6 + self.player.real_acc, self.player.eng_r, [0.0,0.0], world_zoom ])
            else:
                batch.append([ self.priming_sprite, [-8,-6+self.player.acc], 16, self.player.eng_r, [0.0,0.0], world_zoom ])

            batch.append([ self.engine_sprite, [-8,-8+self.player.acc], 4, self.player.eng_r, [0.0,0.0], world_zoom ])

        batch.append([   self.active_player_sprite, [-8,-8], 10+(wobble*3)-(self.player.firing*2), self.player.r, [0.0,0.0], world_zoom ])

        if(dist>100):
            batch.append([   self.reticle_sprite, [-8,-32], 4, reticle_r, [0,0], world_zoom ])

        self.sprite_renderer.render(batch)
        with framebuffer_as_render_target( self.radar_buffer ):
            hwgfx.manual_blend_enter(600)
            self.sprite_renderer.render(batch)
