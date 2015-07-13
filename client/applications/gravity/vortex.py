from math import sin,cos,sqrt

class vortex:
    def __init__(self):
        self.base_rotation = 0.5
        self.dist_scale = 0.02
        pass

    def transform(self, point):
        dist = point[0]*point[0]+point[1]*point[1]
        dist=sqrt(dist)
        r = self.base_rotation * self.dist_scale
        x = point[0] * cos(r) - point[1]*sin(r)
        y = point[0] * sin(r) + point[1]*cos(r)
        return [x,y]


