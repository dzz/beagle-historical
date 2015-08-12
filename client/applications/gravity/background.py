import client.gfx.shaders as shaders
from client.gfx.primitive import primitive, draw_mode
from client.math.helpers import tesselated_unit_quad
from random import uniform,choice

class background():
    def __init__(self):
        self.shaders = [ 
                            shaders.get_client_program( "no_transform","circus"),
                            shaders.get_client_program( "no_transform","castle"),
                            shaders.get_client_program( "no_transform","fortress"),
                            shaders.get_client_program( "no_transform","micrube"),
                            shaders.get_client_program( "no_transform","evil_micrube"),
                            shaders.get_client_program( "no_transform","voodoo"),
                            shaders.get_client_program( "no_transform","screenprint"),
                            shaders.get_client_program( "no_transform","buffer_dupe")
                        ]
        self.primitive = primitive( draw_mode.TRIS, tesselated_unit_quad, tesselated_unit_quad ) 
        self.time = 0
        self.speed = 0.01
        self.x = 0
        self.y = 0
        self.r = 0
        self.shader = self.shaders[0]
        self.randomize_colors()

    def tick(self, speed = None, r = None):
        if(speed is not None):
            self.time += speed*self.speed
        else:
            self.time += self.speed

        if(r is not None):
            self.r = r
        

    def randomize_colors(self):
        self.shader = choice( self.shaders )
        colors = [ 
                        [ 0.5, 0.7, 0.0, 0.5 ],
                        [ 0.4, 0.6, 0.3, 1.0 ],
                        [ 0.2, 0.1, 0.2, 0.3 ],
                        [ 0.0, 0.0, 1.0, 0.5 ],
                        [ 0.0, 0.3, 0.1, 0.8 ],
                        [ 0.5, 0.0, 1.0, 0.7 ],
                        [ 0.0, 0.5, 1.0, 0.2 ],
                        [ 0.3, 0.0, 1.0, 0.3 ],
                        
                        ]
        for i in range(0,7):
            colors.append( [ uniform(0.0,1.0),uniform(0.5,1.0),uniform(0.0,1.0), uniform(0.0,0.1) ] )

        base_color = choice(colors)
        invert_color = [ 1-base_color[0],
                         1-base_color[1],
                         1-base_color[2],
                         uniform(0.0,0.5) ]

        drama = choice([0.0,1.0])
        drama_alpha = choice([0.0,0.5])
        drama_color = [ drama,drama,drama,drama_alpha]

        self.shader.bind([  ("col1",            base_color),
                            ("col3",            invert_color),
                            ("col2",            drama_color),
                            ("circle_factor",   [choice([1,2,3,4])]),
                            ("wobble_factor",   [choice([1,2,3,4])]),
                            ("cosmunge_factor", [choice([0.00001, 1,2,3,99])]),
                            ("steps",           [choice([2,4,8,16,32,64]) ]),
                            ("atan_factor",     [choice([1,2,4,6,7,9,14]) ]),
                            ("crazy",           [uniform(0.1,choice([1.0,2.0,7.0,0.2])) ]),
                            ("r",[self.r ])
                            ] )

    def render(self, world_zoom):
        self.shader.bind([ 
                           ("time", [ self.time ]) ,
                           ("cam", [ self.x,self.y ]) ,
                           ("scale", [ world_zoom ])  
                          ])

        self.primitive.render()

