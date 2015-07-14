from .background import background
from .vortex import vortex
from .player import player
from math import sqrt,atan2,cos,sin
from client.system.gamepad       import get_gamepad
from client.gfx.sprite           import sprite, sprite_renderer
import client.gfx.blend as blend
from client.gfx.tileset          import tileset
from client.gfx.coordinates      import centered_view, Y_Axis_Down
from random import choice, uniform
from client.math.helpers import distance
import hwgfx

class pickup:
    def __init__(self, x,y, player, vortex):
        self.x = x
        self.y = y
        self.player = player
        self.vortex =vortex
        self.level = 1

    def tick(self, particles, sprite_renderer, background):
        d = (self.x-self.player.x)*(self.x-self.player.x)+ (self.y-self.player.y)*(self.y-self.player.y)
        if(d<900):
            background.randomize_colors()
            part_count = 90
            max_spread = d;
            #explosion 1
            for i in range(0,part_count):
                angle = uniform(-3.14,3.14)
                x = sin(angle)
                y = cos(angle)
                d = uniform(60,float(i)/part_count*max_spread)
                vx = x * uniform(5,12)
                vy = y * uniform(5,12)
                x *= d
                y *= d
                part = particle( self.x +x, self.y+y, vx, vy,
                                 angle, sprite_renderer, [13] )
                part.ttl = 300
                particles.append(part)
            self.x = choice([16,32,64,128])*self.level
            self.y = choice([16,32,64,128])*self.level
            self.level*=2
            if(self.level>64):
                if(choice([True,False])):
                    self.level=choice([self.level/2,self.level-32])
            self.vortex.switch_directions()
            #explosion 2
            for i in range(0,part_count):
                angle = uniform(-3.14,3.14)
                x = sin(angle)
                y = cos(angle)
                d = uniform(60,float(i)/part_count*max_spread)
                vx = x * uniform(5,12)
                vy = y * uniform(5,12)
                x *= d
                y *= d
                part = particle( self.x +x, self.y+y, vx, vy,
                                 angle, sprite_renderer, [14] )
                part.ttl = 300
                particles.append(part)


        [self.x,self.y] = self.vortex.transform([self.x,self.y])

class particle:
    def __init__(self, x, y, vx, vy, r, renderer, frames = [ 4,5,6 ] ):
        self.x = x
        self.y = y
        self.vx = vx
        self.vy = vy
        self.r = r
        self.ttl = 60
        self.vdecay = 0.99
        self.sprite = sprite(
                                        sprite_renderer = renderer,
                                        named_animations = {
                                                                "default" : frames
                                                                },
                                        current_animation = "default",
                                        ticks_per_frame = 12
                                     )
    def tick(self, vortex):
        self.r += 0.01
        self.x += self.vx
        self.y += self.vy
        self.vx*=self.vdecay
        self.vy*=self.vdecay
        self.ttl -= 1
        [ self.x, self.y ] = vortex.transform( [ self.x, self.y] )
        self.sprite.tick()



def tick_particles(particles,vortex):
    new_particles = []
    for part in particles:
        part.tick(vortex)
        if(part.ttl>0):
            new_particles.append(part)
    return new_particles

class game:
    def __init__(self):
       self.t = 0
       self.background = background()
       self.vortex = vortex()
       self.player = player()
       self.pickup = pickup(16,16,self.player,self.vortex)
       self.particles = []

       configuration = {
                "image"         : "ship.png",
                "imageheight"   : 48,
                "imagewidth"    : 112,
                "margin"        : 0,
                "spacing"       : 0,
                "properties"    : {},
                "firstgid"      : 0,
                "tileheight"    : 16,
                "tilewidth"     : 16,
                "tileproperties" : {} 
                }

       self.sprite_tileset  = tileset( configuration = configuration, img_path = "sprite/" )

       self.sprite_renderer = sprite_renderer( tileset = self.sprite_tileset, coordinates = centered_view(1920,1080, Y_Axis_Down))
       self.priming_sprite = sprite(
                                        sprite_renderer = self.sprite_renderer,
                                        named_animations = {
                                                                "default" : [11,12]
                                                                },
                                        current_animation = "default",
                                        ticks_per_frame = 3
                                     )
       self.reticle_sprite = sprite(
                                        sprite_renderer = self.sprite_renderer,
                                        named_animations = {
                                                                "default" : [10]
                                                                },
                                        current_animation = "default",
                                        ticks_per_frame = 1
                                     )
       self.player_sprite = sprite(
                                        sprite_renderer = self.sprite_renderer,
                                        named_animations = {
                                                                "default" : [0]
                                                                },
                                        current_animation = "default",
                                        ticks_per_frame = 1
                                     )
       self.engine_sprite = sprite(
                                        sprite_renderer = self.sprite_renderer,
                                        named_animations = {
                                                                "default" : [1]
                                                                },
                                        current_animation = "default",
                                        ticks_per_frame = 1
                                     )
       self.fire_sprite = sprite(
                                        sprite_renderer = self.sprite_renderer,
                                        named_animations = {
                                                                "default" : [2,3]
                                                                },
                                        current_animation = "default",
                                        ticks_per_frame = 5
                                     )

       self.emerald_sprite = sprite(
                                        sprite_renderer = self.sprite_renderer,
                                        named_animations = {
                                                                "default" : [7,8,9,8]
                                                                },
                                        current_animation = "default",
                                        ticks_per_frame = 9
                                     )

    def tick(self):
       self.t +=1
       if(self.t%choice([3,5,9])==0 and self.player.firing>0):
           self.particles.append( particle( 
                    self.player.x - self.player.vx,
                    self.player.y - self.player.vy,
                    0,0,
                    self.player.r,
                    self.sprite_renderer
               ))

       self.pickup.tick(self.particles, self.sprite_renderer, self.background )
       self.particles = tick_particles(self.particles,self.vortex)
       pad         = get_gamepad(0)
       #print( pad.left_stick);
       self.background.update(self.vortex.td_current)
       self.priming_sprite.tick()
       self.emerald_sprite.tick()
       self.player.tick(pad,self.vortex)
       self.fire_sprite.tick()
       self.background.x = self.player.x
       self.background.y = self.player.y

    def render(self):
        pad = get_gamepad(0)
        wobble = (sin (self.t/12)+1)/2.0

        world_zoom_min = 0.66
        world_zoom_max = 1.0
        target_x = self.pickup.x-self.player.x
        target_y = self.pickup.y-self.player.y
        dist = sqrt(target_x*target_x+target_y*target_y)
        reticle_r = 3.14-atan2(target_x,target_y)

        print(distance(0,0,self.player.x,self.player.y))

        world_zoom = world_zoom_max -(pad.triggers[0]*(world_zoom_max-world_zoom_min))
        self.background.render(world_zoom)

        batch  = [];
        shadow_batch = []
        particle_batch = []

        if(dist>10):

            x1 = self.player.x
            y1 = self.player.y
            x2 = self.pickup.x
            y2 = self.pickup.y

            length = max(1,distance(x1,y1,x2,y2)) / 512

            x1/=length
            x2/=length
            y1/=length
            y2/=length


            shadow_batch.append([   

                self.player_sprite,
                [0,-8],
                12+wobble*2,
                -wobble,
                [x1,y1],
                1.0 
                ])

            shadow_batch.append([   

                self.emerald_sprite,
                [-8,-8],
                8+wobble*4,
                wobble,
                [x2,y2],
                1.0 
                ])

            shadow_batch.append([   

                self.player_sprite,
                [-8,-8],
                -9,
                0+reticle_r,
                [x1,y1],
                -1.0 
                ])

            shadow_batch.append([   

                self.emerald_sprite,
                [0,-8],
                -9,
                0-reticle_r,
                [x2,y2],
                -1.0 
                ])


        for part in self.particles:
            particle_batch.append([
                 part.sprite,
                 [-8,-8],
                 4+(part.r*2),
                 part.r,
                 [part.x-self.player.x,part.y-self.player.y],
                 world_zoom ])

        hwgfx.manual_blend_enter(600)
        self.sprite_renderer.render(shadow_batch)
        hwgfx.manual_blend_enter(5000)
        self.sprite_renderer.render(particle_batch)
        hwgfx.manual_blend_enter(0)

        batch.append([   

            self.emerald_sprite,
            [-8,-8],
            3+(wobble*3),
            atan2(self.pickup.x,self.pickup.y),
            [self.pickup.x-self.player.x,self.pickup.y-self.player.y],
            world_zoom 
            ])

        if(self.player.firing>0):
            if(self.player.real_acc>0.98):
                batch.append([

                     self.fire_sprite,
                     [-8,-2+self.player.acc],
                     4 + self.player.real_acc,
                     self.player.eng_r,
                     [0.0,0.0],
                     world_zoom ])
            else:
                batch.append([

                     self.priming_sprite,
                     [-8,-2+self.player.acc],
                     6,
                     self.player.eng_r,
                     [0.0,0.0],
                     world_zoom ])

            batch.append([

                 self.engine_sprite,
                 [-8,-8+self.player.acc],
                 4,
                 self.player.eng_r,
                 [0.0,0.0],
                 world_zoom ])

        batch.append([   

            self.player_sprite,
            [-8,-8],
            2+max(1,(self.player.vx+self.player.vy)),
            self.player.r,
            [0.0,0.0],
            world_zoom 
            ])




        if(dist>100):
            batch.append([   

                self.reticle_sprite,
                [-8,-32],
                4,
                reticle_r,
                [0,0],
                world_zoom 
                ])







        self.sprite_renderer.render(batch)



global g
def init():
    global g
    g = game()

def tick():
    g.tick()

def render():
    g.render()

def finalize():
    pass

def configure( configuration ):
    pass
