import client.gfx.shaders as shaders
from client.gfx.primitive import primitive, draw_mode
from client.math.helpers import tesselated_unit_quad

class background():
    def __init__(self):
        # load the background plasma shader pipeline
        #
        self.shader = shaders.get_client_program( "no_transform","background")

        # make a basic -1+1 square primitive using client.math.helpers tesselated quad
        # template for both vertices and uv coordinates
        #
        # since the shader's render target is always a unit square, using a unit square 
        # as geometry is a simple way to do all of your rendering in the fragment shader
        # itself (shader/sbackground.glsl)
        #
        self.primitive = primitive( draw_mode.TRIS, tesselated_unit_quad, tesselated_unit_quad ) 

        # we're going to tick time and use that as input to the plasma shader
        #
        self.time = 0
        self.speed = 0.01

    def update(self):
        self.time += self.speed

    def render(self):
        # bind our internal time property to the shader, 
        #
        self.shader.bind( [ ("time", [ self.time ] ) ] )
        # and render our unit quad
        #
        self.primitive.render()

