from client.system.audio import beatEngine
from math import atan2, sqrt, sin, cos, floor
from random import choice
import client.ctt2.host_config  as host_config
import audio

class player:
    def __init__(self):
        self.dummy_clips = [
                audio.clip_create(host_config.get("app_dir") + "audio/stb00.ogg"),
                #audio.clip_create(host_config.get("app_dir") + "audio/stb01.ogg"),
                #audio.clip_create(host_config.get("app_dir") + "audio/stb02.ogg"),
                #audio.clip_create(host_config.get("app_dir") + "audio/stb03.ogg"),
                #audio.clip_create(host_config.get("app_dir") + "audio/stb04.ogg")
                ]

        self.fire_clips = [
                audio.clip_create(host_config.get("app_dir") + "audio/hh00.ogg"),
                #audio.clip_create(host_config.get("app_dir") + "audio/hh01.ogg"),
                #audio.clip_create(host_config.get("app_dir") + "audio/hh02.ogg"),
                #audio.clip_create(host_config.get("app_dir") + "audio/hh03.ogg"),
                #audio.clip_create(host_config.get("app_dir") + "audio/hh04.ogg")
                ]

        #audio.track_play_clip(1, self.dummy_clips[0] )
        #audio.track_play_clip(2, self.fire_clips[0] )
        self.vx = 0
        self.vy = 0
        self.x = 0
        self.y = 0
        self.acc = 0
        self.vdecay = 0.99
        self.vr = [0.0,0.0]
        self.r = 0
        self.eng_r = 0
        self.firing = 0
        self.fuzzy_firing_state = 0
        self.real_acc = 0
        self.speed = 0.0
        self.points = 10000

    def tick(self, pad,vortex,music_system):
        ox = self.x
        oy = self.y
        ovx = self.vx
        ovy = self.vy
        ofiring = self.firing

        self.eng_r = 3.14-atan2(pad.left_stick[0],pad.left_stick[1])

        beatEngine.set_backdoor(0, self.r/6.28)
        beatEngine.set_backdoor(1, self.vx ) 
        beatEngine.set_backdoor(2, self.acc )
        
        self.acc = pad.triggers[1]+1
        fuzzy_firing_impulse = 0.0
        if(self.acc>0):
            fuzzy_firing_impulse = 1.0
            self.firing = self.acc
            self.real_acc = (0.93*self.real_acc)+(0.07*self.acc*1)
            self.vx += (pad.left_stick[0]*(self.real_acc*self.real_acc))/8;
            self.vy += (pad.left_stick[1]*(self.real_acc*self.real_acc))/8;
        else:
            self.vx*=self.vdecay
            self.vy*=self.vdecay
            self.firing = 0

        if( ((self.firing==0) and (ofiring!=0)) or
            ((self.firing!=0) and (ofiring==0))):
                audio.track_play_clip(2, choice(self.fire_clips) )


        self.fuzzy_firing_state = (0.7*self.fuzzy_firing_state)+(0.3*fuzzy_firing_impulse)
        music_system.track_volume("DrumEffects",self.fuzzy_firing_state)
        self.x += self.vx
        self.y += self.vy

        [self.x,self.y] = vortex.transform([self.x,self.y])

        a = 0.2
        b = 0.8
        [self.vr[0],self.vr[1]] = [
                                    (self.x-ox)*a + self.vr[0]*b,
                                    (self.y-oy)*a + self.vr[1]*b ]

        self.r = 3.14 - atan2( self.vr[0]*0.25+pad.left_stick[0]*0.75,self.vr[1]*0.25+pad.left_stick[1]*0.75 )

        delta_x = self.x - ox
        delta_y = self.y - oy
        self.speed = floor(sqrt((delta_x*delta_x)+(delta_y*delta_y))*10)*5

        if ( ( (self.vx>0) and (ovx<0) ) or
           ( (self.vy>0) and (ovy<0) ) or
           ( (self.vx<0) and (ovx>0) ) or
           ( (self.vy<0) and (ovy>0) ) ):
               audio.track_play_clip(1,choice(self.dummy_clips))
