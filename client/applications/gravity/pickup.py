from .background import background
from .vortex import vortex
from .player import player
from .particle import particle
from math import sqrt,atan2,cos,sin
from client.system.gamepad       import get_gamepad
from client.gfx.sprite           import sprite, sprite_renderer
import client.gfx.blend as blend
from client.gfx.tileset          import tileset
from client.gfx.coordinates      import centered_view, Y_Axis_Down
from random import choice, uniform, sample
from client.math.helpers import distance
from .music_system import music_system
import hwgfx
from random import sample

class pickup:
    def __init__(self, x,y, player, vortex, level_observer ):

        self.level_observer = level_observer
        self.particle_blend_mode = 5000
        self.has_leads = True
        self.drums_dynamic_volume = True
        self.x = x
        self.y = y
        self.player = player
        self.vortex =vortex
        self.level = 1
        self.t = 0
        self.radars_wobble = False
        self.level_incr_amt = 0.25
        self.levelled = False
        self.max_level = 32
        self.level_tier_increase = 5
        self.collection_envelope_follower = 0.0
        self.collection_envelope_follower_decay = 0.998
        self.collection_envelope_follower_smoothed = 0.0
        self.adaptive_max_d = 0.01
        self.d_nrml_smoothed = 0.01

    def tick(self, particles, sprite_renderer, background, music_system ):
        self.collection_envelope_follower = self.collection_envelope_follower * self.collection_envelope_follower_decay
        self.collection_envelope_follower_smoothed = 0.99*self.collection_envelope_follower_smoothed + 0.01 * self.collection_envelope_follower
        music_system.track_volume("Bass", self.collection_envelope_follower)
        music_system.track_volume("Pads", 1-self.collection_envelope_follower)
        if(self.drums_dynamic_volume):
            music_system.track_volume("DrumStatic", 1-(self.collection_envelope_follower_smoothed*self.collection_envelope_follower_smoothed))
        else:
            music_system.track_volume("DrumStatic", 1);

        self.levelled = False
        self.t +=1

        d = (self.x-self.player.x)*(self.x-self.player.x)+ (self.y-self.player.y)*(self.y-self.player.y)
        real_d = sqrt(d)
        pickup_sound_x = (self.player.x-self.x)/real_d
        pickup_sound_y = (((self.y-self.player.y)/real_d)+1)*0.5

        music_system.track_volume("PickupHi", 1-pickup_sound_y)
        music_system.track_volume("PickupLow",pickup_sound_y)
        music_system.track_pan("PickupHi", pickup_sound_x)
        music_system.track_pan("PickupLow",pickup_sound_x)


        d_adaptable = d*1.15
        if(d_adaptable>self.adaptive_max_d):
            self.adaptive_max_d = d_adaptable
        else:
            self.adaptive_max_d*=0.995

        d_nrml = d/self.adaptive_max_d
        self.d_nrml_smoothed = self.d_nrml_smoothed*0.99+d_nrml*0.01

        #print("Setting pickup audio cue to:{0}".format(1-d_nrml))
        if(self.has_leads):
            music_system.track_volume("Lead", 1-(self.d_nrml_smoothed)*(1-self.collection_envelope_follower))
        else:
            music_system.track_volume("Lead",0.0);


        if(d<2500):
            self.level_observer.level_up()
            self.particle_blend_mode = choice([5000,6000])
            self.has_leads = choice([True,False,False])
            self.drums_dynamic_volume = choice([True,False,False])
            self.collection_envelope_follower = 1.0
            music_system.trigger_event("level_up")
            self.levelled = True
            background.randomize_colors()
            part_count = 250
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
                part = particle( self.x +x, self.y+y, vx, vy, angle, "pickup_explosion")
                part.ttl = 300
                particles.append(part)
            self.x = choice([16,32,64,72])*self.level
            self.y = choice([16,32,64,72])*self.level
            self.level*=1.0+self.level_incr_amt
            self.level_incr_amt*=0.91

           # self.radars_wobble = choice([True,False,False])
            if(self.level>self.max_level):
                if(choice([True,False])):
                    self.level=choice([self.level/2,self.level-32])
                    self.max_level += self.level_tier_increase
                    self.level_incr_amt = 0.15
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
                part = particle( self.x +x, self.y+y, vx, vy, angle, "pickup_explosion")
                part.ttl = 300
                particles.append(part)


        [self.x,self.y] = self.vortex.transform([self.x,self.y])
