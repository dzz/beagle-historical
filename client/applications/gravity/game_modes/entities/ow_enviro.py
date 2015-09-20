from client.ctt2.assets import assets
from math import sin

class ow_enviro:
    def __init__(self,config ):
        self.x_range = [config["room_range"]["left"],config["room_range"]["right"]]
        self.children = []

    def register_child(self,child):
        self.children.append(child)

    def handle_wrap_around(self,child):
        if child.x < self.x_range[0]:
            child.x = self.x_range[1]
        if child.x > self.x_range[1]:
            child.x = self.x_range[0]


    def get_rotated(self,x, scale = 1.0, phase = 0.0):
        x -= self.x_range[0]
        x += phase
        x /= (self.x_range[1]-self.x_range[0])
        x *= 3.14159 * 2
        return sin(x + (phase*3.14159) ) * scale



    def tick(self):
        for child in self.children:
            self.handle_wrap_around(child)
