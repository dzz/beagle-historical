from client.gfx.primitive import primitive,draw_mode
import client.gfx.shaders as shaders


class sprite():
    def __init__(self, sprite_renderer, named_animations = { "default" : [0] } , current_animation = "default", ticks_per_frame = 2, size = None ):
        self.named_animations = named_animations
        self.frame_index = 0
        self.ticks = 0
        self.current_animation = current_animation
        self.ticks_per_frame = ticks_per_frame
        self.primitives = {}
        self.sprite_renderer = sprite_renderer
        self.size = size

        if self.size is None:
            self.size = self.sprite_renderer.tileset.tileheight

        self.compile()

    def compile(self):
        for animation in self.named_animations:
            self.primitives[animation] = []
            for frame in self.named_animations[animation]:
                print("trying to compile frame:{0}".format(frame))
                tile = self.sprite_renderer.tileset.get_gid( frame )
                if not tile:
                    continue
                print("------compiling---")
                sz = self.size
                self.primitives[animation].append( primitive( draw_mode.TRIS, [ 
                                                                     [ 0.0,    0.0,  ],
                                                                     [ 0.0+sz, 0.0,  ],
                                                                     [ 0.0+sz, 0.0+sz],
                                                                     [ 0.0+sz, 0.0+sz],
                                                                     [ 0.0,    0.0+sz],
                                                                     [ 0.0,    0.0   ]  ],
                                                           [
                                                            [ tile[0],         tile[1]         ],
                                                            [ tile[0]+tile[2], tile[1]         ],
                                                            [ tile[0]+tile[2], tile[1]+tile[3] ],
                                                            [ tile[0]+tile[2], tile[1]+tile[3] ],
                                                            [ tile[0]        , tile[1]+tile[3] ],
                                                            [ tile[0],         tile[1]         ] ]) )

    def select_animation( self, key ):
        self.current_animation = key
        self.frame_index = 0

    def tick(self):
        self.ticks = (self.ticks + 1) % (self.ticks_per_frame)
        if(self.ticks == 0 ):
            self.frame_index = (self.frame_index + 1) % len( self.named_animations[self.current_animation] )

    def get_current_primitive(self):
        return self.primitives[self.current_animation][self.frame_index]
        

class sprite_renderer():
    def __init__( self, tileset, coordinates = [1.0,1.0],):
        self.tileset = tileset
        self.coordinates = coordinates
        self.shader = shaders.get_unique( "hwgfx/tilemap", "hwgfx/tilemap" )

    def render(self, sprite_render_operations):
        self.tileset.texture.bind(0)
        self.shader.bind([ ("view", self.coordinates) ])
        for sprite_render_operation in sprite_render_operations:
            sprite = sprite_render_operation[0]
            translation = sprite_render_operation[1]
            scale = sprite_render_operation[2]
            self.shader.bind([ ("scale", [scale]), ("translation", translation ) ])
            sprite.get_current_primitive().render()


