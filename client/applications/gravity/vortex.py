from math import sin,cos,sqrt
from random import uniform,choice
import client.beagle.beagle_environment  as beagle_environment
import audio

class vortex:
    def __init__(self):
        self.dummy_clips = [
                audio.clip_create(beagle_environment.get("app_dir") + "audio/vt00.ogg"),
                #audio.clip_create(host_config.get("app_dir") + "audio/vt01.ogg"),
                #audio.clip_create(host_config.get("app_dir") + "audio/vt02.ogg"),
                #audio.clip_create(host_config.get("app_dir") + "audio/vt03.ogg"),
                #audio.clip_create(host_config.get("app_dir") + "audio/vt04.ogg")
                ]
        self.base_rotation = 0.5
        self.dist_scale = 0.02
        self.td = 1.0
        self.td_current = 1.0
        self.radscale = False
        self.use_phase = False
        self.vortex_phases = False
        self.time = 0.0 
        self.time_scale = 1.0
        self.active = False
        self.switched_count = 0
        self.ortho = False
        pass

    def switch_directions(self):
        self.switched_count +=1
        if self.switched_count>15:
            self.active=True
            audio.track_play_clip(3, self.dummy_clips[0] )
        if self.active:
            self.td = self.td*-1
            self.dist_scale *= uniform(0.8,1.2)
            self.time_scale = choice([0.5,1.0,1.5])

            if uniform(0.0,1.0)>0.75:
                audio.track_play_clip(3, choice(self.dummy_clips))
                self.time_scale = 1

            if uniform(0.0,1.0)>0.6:
                audio.track_play_clip(3, choice(self.dummy_clips))
                self.ortho = True
            else:
                self.ortho = False

            self.radscale = choice([True,False,False,False])
            self.use_phase = choice([True,False,False])
            self.vortex_phases = choice([True,False])

            if(self.vortex_phases):
                audio.track_play_clip(3, choice(self.dummy_clips))

    def transform(self, point):
        if not self.active:
            return point
        self.time+=self.td_current*self.time_scale
        self.td_current = (self.td_current * 0.999) + (self.td*0.001)
        dist = point[0]*point[0]+point[1]*point[1]
        dist=sqrt(dist)
        r = self.base_rotation * self.dist_scale * self.td_current
        if(self.radscale):
            #r += (sin(dist/300.0)*0.001)
            pass
        if(self.use_phase):
            #r*=(1+sin(self.time*0.001))/2.
            pass

        x = point[0] * cos(r) - point[1]*sin(r)
        y = point[0] * sin(r) + point[1]*cos(r)

        if self.vortex_phases:
            phase= min((sin(self.time*0.0001)),0.0)

            x = x*phase + point[0]*(1-phase)
            y = y*phase + point[1]*(1-phase)
        if not self.ortho:
            return [x,y]
        else:
            if(abs(x-point[0])>abs(y-point[1])):
                return [x,point[1]]
            else:
                return [point[0],y]



