from client.math.helpers import lerp_vec
from math import tanh
class curve:
    def __init__(self,points):
        self.points = points

    def value_at(self,t):
        left = None
        right = None
        for point in self.points:
            if point["t"] <t:
                left = point
            if point["t"] >t:
                right = point
                break

        if left and right:
            nt = (t - left["t"]) / (right["t"]-left["t"])
            if("smooth" in right) and right["smooth"]:
                if right["smooth"] == "tanh":
                    nt = tanh(nt*3.14)
                if right["smooth"] == "sqr":
                    nt = nt*nt
            return lerp_vec(left["vec"],right["vec"],nt)

        if left:
            return left["vec"]

        if right:
            return right["vec"]
            
        return self.points[0]["vec"]

