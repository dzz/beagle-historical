from math import sin,cos,sqrt
from random import uniform,choice

class vortex:
    def __init__(self):
        self.base_rotation = 0.5
        self.dist_scale = 0.02
        self.td = 1.0
        self.td_current = 1.0
        self.radscale = False
        self.use_phase = False
        self.time = 0.0 
        self.time_scale = 1.0
        pass

    def switch_directions(self):
        self.td = self.td*-1
        self.dist_scale *= uniform(0.5,2)
        self.time_scale = choice([0.5,1.0,1.5])
        self.radscale = choice([True,False,False,False])
        self.use_phase = choice([True,False,False])

    def transform(self, point):
        self.time+=self.td_current*self.time_scale
        self.td_current = (self.td_current * 0.999) + (self.td*0.001)
        dist = point[0]*point[0]+point[1]*point[1]
        dist=sqrt(dist)
        r = self.base_rotation * self.dist_scale * self.td_current
        if(self.radscale):
            r += sin(dist/300.0)*0.1
        if(self.use_phase):
            r*=sin(self.time*0.0001)

        x = point[0] * cos(r) - point[1]*sin(r)
        y = point[0] * sin(r) + point[1]*cos(r)
        return [x,y]


