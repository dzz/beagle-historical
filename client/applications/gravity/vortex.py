from math import sin,cos,sqrt
from random import uniform

class vortex:
    def __init__(self):
        self.base_rotation = 0.5
        self.dist_scale = 0.02
        self.td = 1.0
        self.td_current = 1.0
        pass

    def switch_directions(self):
        self.td = self.td*-1
        self.dist_scale *= uniform(0.5,2)

    def transform(self, point):
        self.td_current = (self.td_current * 0.999) + (self.td*0.001)
        dist = point[0]*point[0]+point[1]*point[1]
        dist=sqrt(dist)
        r = self.base_rotation * self.dist_scale * self.td_current
        x = point[0] * cos(r) - point[1]*sin(r)
        y = point[0] * sin(r) + point[1]*cos(r)
        return [x,y]


