import client.gfx.shaders as shaders
from client.gfx.primitive import primitive, draw_mode
from client.math.helpers import tesselated_unit_quad
from random import uniform,choice

class background():
    def __init__(self):
        self.shader = shaders.get_client_program( "no_transform","background")
        self.primitive = primitive( draw_mode.TRIS, tesselated_unit_quad, tesselated_unit_quad ) 
        self.time = 0
        self.speed = 0.01
        self.x = 0
        self.y = 0
        self.randomize_colors()

    def update(self, speed = None):
        if(speed is not None):
            self.time += speed*self.speed
        else:
            self.time += self.speed
        

    def randomize_colors(self):

        colors = [ 
                        [ 1.0, 0.0, 0.0, 1.0 ],
                        [ 0.0, 1.0, 0.0, 1.0 ],
                        [ 0.0, 0.0, 1.0, 1.0 ],
                        [ 0.0, 1.0, 1.0, 1.0 ],
                        [ 0.0, 0.0, 0.0, 0.0 ],
                        [ 1.0, 1.0, 1.0, 1.0 ],
                        [ 0.0, 0.0, 0.0, 1.0 ] ]
        base_color = choice(colors)
        invert_color = [ 1-base_color[0],
                         1-base_color[0],
                         1-base_color[0],
                         1-base_color[0] ]
        drama = choice([0.0,1.0])
        drama_color = [ drama,drama,drama,1.0]

        self.shader.bind([("col1",base_color)])
        self.shader.bind([("col2",invert_color)])
        self.shader.bind([("col3",drama_color)])
        self.shader.bind([("circle_factor",[choice([1,2,3,4])])])
        self.shader.bind([("wobble_factor",[choice([1,2,3,4])])])
        self.shader.bind([("cosmunge_factor",[choice([0.00001, 1,24,3,99])])])
        self.shader.bind([("steps",[ choice([2,4,6,7,9,14,64]) ])])
        self.shader.bind([("atan_factor",[ choice([2,4,6,7,9,14,64]) ])])

    def render(self, world_zoom):
        self.shader.bind( [ ("time", [ self.time ] ) ] )
        self.shader.bind( [ ("cam", [ self.x,self.y ] ) ] )
        self.shader.bind( [ ("scale", [ world_zoom ] ) ] )
        self.primitive.render()

