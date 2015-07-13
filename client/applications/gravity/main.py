from .background import background
from .vortex import vortex
from .player import player
from math import sqrt,atan2
from client.system.gamepad       import get_gamepad
from client.gfx.sprite           import sprite, sprite_renderer
from client.gfx.tileset          import tileset
from client.gfx.coordinates      import centered_view, Y_Axis_Down
from random import choice

class pickup:
    def __init__(self, x,y, player, vortex):
        self.x = x
        self.y = y
        self.player = player
        self.vortex =vortex
        self.level = 1

    def tick(self):
        d = (self.x-self.player.x)*(self.x-self.player.x)+ (self.y-self.player.y)*(self.y-self.player.y)
        if(d<60):
            self.x = choice([16,32,64,128])*self.level
            self.y = choice([16,32,64,128])*self.level
            self.level*=2

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
        self.x += self.vx
        self.y += self.vy
        self.vx*=self.vdecay
        self.vy*=self.vdecay
        self.ttl -= 1
        #[ self.x, self.y ] = vortex.transform( [ self.x, self.y] )
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
                "imageheight"   : 32,
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

       self.pickup.tick()
       self.particles = tick_particles(self.particles,self.vortex)
       pad         = get_gamepad(0)
       #print( pad.left_stick);
       self.background.update()
       self.emerald_sprite.tick()
       self.player.tick(pad,self.vortex)
       self.fire_sprite.tick()
       self.background.x = self.player.x
       self.background.y = self.player.y

    def render(self):
        self.background.render()

        batch  = [];

        for part in self.particles:
            batch.append([
                 part.sprite,
                 [-8,-8],
                 4,
                 part.r,
                 [part.x-self.player.x,part.y-self.player.y],
                 2 ])

        batch.append([   

            self.emerald_sprite,
            [-8,-8],
            3,
            atan2(self.pickup.x,self.pickup.y),
            [self.pickup.x-self.player.x,self.pickup.y-self.player.y],
            2 
            ])

        if(self.player.firing>0):
            batch.append([

                 self.fire_sprite,
                 [-8,-2+self.player.acc],
                 6,
                 self.player.eng_r,
                 [0.0,0.0],
                 2 ])
            batch.append([

                 self.engine_sprite,
                 [-8,-8+self.player.acc],
                 4,
                 self.player.eng_r,
                 [0.0,0.0],
                 2 ])

        batch.append([   

            self.player_sprite,
            [-8,-8],
            4,
            self.player.r,
            [0.0,0.0],
            2 
            ])


        target_x = self.pickup.x-self.player.x
        target_y = self.pickup.y-self.player.y

        dist = sqrt(target_x*target_x+target_y*target_y)
        reticle_r = 3.14-atan2(target_x,target_y)

        if(dist>100):
            batch.append([   

                self.reticle_sprite,
                [-8,-32],
                4,
                reticle_r,
                [0,0],
                2 
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
