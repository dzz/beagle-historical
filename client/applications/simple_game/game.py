from client.gfx.coordinates      import centered_view, Y_Axis_Down
from client.gfx.sprite           import sprite, sprite_renderer
from client.gfx.tilemap          import tilemap
import client.gfx.context        as gfx_context
from client.system.gamepad       import get_gamepad
from client.gfx.tileset          import tileset
from .player import player

class game:
    def __init__(self):
        self.player_sprite = None
        self.coord_system = centered_view(1920,1080,Y_Axis_Down)
        self.load_sprites()
        self.load_tilemap()
        self.player = player( self )

        #
        #setting unit size to 16 - tilemap/sprites are 16x16 and we're 
        #letting the tilemap/sprite compilers use the native size for
        #the geometry.
        
        #this is just a number that we to convert a number like players location
        #(which is in 'tilespace') into the right range for 'renderspace'
        #

        self.unit_size = 16.0   

        self.magnification = 8
        gfx_context.set_clear_color(1.0,1.0,1.0,0.0)

    def get_tile(self, x, y):
        return self.tilemap.get_layer_tile(0, x,y)

    def load_sprites(self):
        configuration = {
                "image"         : "smashtiles.png",
                "imageheight"   : 80,
                "imagewidth"    : 64,
                "margin"        : 0,
                "spacing"       : 0,
                "properties"    : {},
                "firstgid"      : 1,
                "tileheight"    : 16,
                "tilewidth"     : 16,
                "tileproperties" : {} 
                }

        self.sprite_tileset  = tileset( configuration = configuration, img_path = "tiles/" )
        self.sprite_renderer = sprite_renderer( tileset = self.sprite_tileset, coordinates = self.coord_system )
        self.player_sprite = sprite( 
                                    sprite_renderer = self.sprite_renderer, 
                                    named_animations = { 
                                                            "up"   : [4, 5, 6 ], 
                                                            "left" : [7, 8, 9 ], 
                                                            "down" : [10, 11,12], 
                                                            "right": [13,14,15]
                                                       }, 
                                    current_animation = "up",
                                    ticks_per_frame = 10 )

    def load_tilemap(self):
        self.tilemap = tilemap.from_json_file( "map/room.json", "tiles/", coordinates = self.coord_system )

    def update(self):
        self.player_sprite.tick()
        self.player.update()

    def render(self):
        gfx_context.clear()

        #render the tilemap, setting the origin to the inverse of the players position, multiplied by
        #the worlds unit size to map the players 'logical' location to something that makes
        #more sense to render
        self.tilemap.render( -self.player.x*self.unit_size, -self.player.y*self.unit_size, self.magnification ) 

        #-8,-8, to place the sprite dead center
        self.sprite_renderer.render([ [ self.player_sprite, [ -8.0,-8.0 ],self.magnification ] ] )
                        
