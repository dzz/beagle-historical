from client.gfx.coordinates      import centered_view, Y_Axis_Down
from client.gfx.sprite           import sprite, sprite_renderer
from client.gfx.tilemap          import tilemap
import client.gfx.context        as gfx_context
from client.system.gamepad       import get_gamepad
from client.gfx.tileset          import tileset
from math import sin
from .player import player
from .ghost import ghost, SLEEPING
from .ball import ball
from .background import background

class game:
    def __init__(self):
        self.background = background()
        self.player_sprite = None
        self.ghost_sprite = None
        self.sleeping_ghost_sprite = None
        self.ball_sprite = None
        self.coord_system = centered_view(1920,1080,Y_Axis_Down)
        self.load_sprites()
        self.load_tilemap()
        self.player = player( self )
        self.ghosts = self.create_ghosts()
        self.ball = ball(self, 16, 14 )
        self.ball_scale = 1

        self.wobble_index = 0.0   #use this to drive an oscilator to wobble the scale of our ghosts
        self.wobble_speed = 0.09
        self.ghost_scale = 1

        self.player_score = 0
        self.ghosts_score = 0

        #
        #setting unit size to 16 - tilemap/sprites are 16x16 and we're 
        #letting the tilemap/sprite compilers use the native size for
        #the geometry.
        
        #this is just a number that we to convert a number like players location
        #(which is in 'tilespace') into the right range for 'renderspace'
        #

        self.unit_size = 16.0   

        self.world_magnification = 4
        gfx_context.set_clear_color(1.0,1.0,1.0,0.0)

    def create_ghosts(self):
        return [
                    ghost( self, x=7, y=8 ),
                    ghost( self, x=25, y=8 ),
                    ghost( self, x=7, y=25 ),
                    ghost( self, x=25, y=25 )
               ]


    def get_tile(self, x, y):
        return self.tilemap.get_layer_tile(0, x,y)

    def load_sprites(self):
        configuration = {
                "image"         : "smashtiles.png",
                "imageheight"   : 96,
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
                                                            "up"   : [4, 5, 6, 5 ], 
                                                            "left" : [7, 8, 9, 8 ], 
                                                            "down" : [10, 11,12, 11], 
                                                            "right": [13,14,15, 14]
                                                       }, 
                                    current_animation = "up",
                                    ticks_per_frame = 7 )

        self.ghost_sprite = sprite( 
                                    sprite_renderer = self.sprite_renderer, 
                                    named_animations = { 
                                                            "wobble"   : [18, 19 ], 
                                                       }, 
                                    current_animation = "wobble",
                                    ticks_per_frame = 13 )

        self.sleeping_ghost_sprite = sprite( 
                                    sprite_renderer = self.sprite_renderer, 
                                    named_animations = { 
                                                            "sleeping"   : [ 17 ], 
                                                       }, 
                                    current_animation = "sleeping" )

        self.ball_sprite = sprite( 
                                    sprite_renderer = self.sprite_renderer, 
                                    named_animations = { 
                                                            "ball"   : [16], 
                                                       }, 
                                    current_animation = "ball")

    def load_tilemap(self):
        self.tilemap = tilemap.from_json_file( "map/room.json", "tiles/", coordinates = self.coord_system )
        self.world_size = [ self.tilemap.layers[0]["width"], self.tilemap.layers[0]["height"] ]

    def update(self):

        self.background.update()
        #as time increases, the input to the sin function changes
        self.wobble_index += self.wobble_speed
        self.ghost_scale = 1.25 + ((sin( self.wobble_index))*0.15)

        self.player_sprite.tick()
        self.ghost_sprite.tick()
        self.player.update()
        self.ball.update()
        for ghost in self.ghosts:
            ghost.update()


    def render(self):
        self.background.render()

        #render the tilemap, setting the origin to the inverse of the players position, multiplied by
        #the worlds unit size to map the players 'logical' location to something that makes
        #more sense to render
        self.tilemap.render( -self.player.x*self.unit_size, -self.player.y*self.unit_size, self.world_magnification ) 

        #-8,-8, to place the sprite dead center

        batch = []
        batch.append( [self.player_sprite, [ -8.0,-8.0 ] ,1 + self.player.kicking_power, [0.0,0.0], self.world_magnification ] )
        batch.append( [self.ball_sprite, [-8.0,-8.0],self.ball_scale, [ (self.ball.x-self.player.x)* self.unit_size, (self.ball.y-self.player.y)*self.unit_size], self.world_magnification] )
        for ghost in self.ghosts:
            if(ghost.state == SLEEPING):
                sprite = self.sleeping_ghost_sprite
            else:
                sprite = self.ghost_sprite

            batch.append( [sprite, [ -8.0,-8.0 ],self.ghost_scale, [ (ghost.fx-self.player.x) * self.unit_size,(ghost.fy-self.player.y)*self.unit_size], self.world_magnification ] )
        self.sprite_renderer.render(batch)
                        
