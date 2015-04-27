import client.gfx.shaders as shaders
from client.gfx.primitive import primitive, draw_mode
from client.math.helpers import tesselated_unit_quad

class background():
    def __init__(self):
        self.shader = shaders.get_client_program( "no_transform","background")


        self.primitive = primitive( draw_mode.TRIS, tesselated_unit_quad, tesselated_unit_quad ) 
        self.time = 0
        self.speed = 0.04



    def update(self):
        self.time += self.speed

    def render(self):
        self.shader.bind( [ ("time", [ self.time ] ) ] )
        self.primitive.render()

