from client.gfx.coordinates      import centered_view, Y_Axis_Down
from client.gfx.sprite           import sprite, sprite_renderer
from client.gfx.tilemap          import tilemap
import client.gfx.context        as gfx_context
from client.system.gamepad       import get_gamepad
from client.gfx.tileset          import tileset
from math import sin
from .player import player
from .ghost import ghost, ai_state
from .ball import ball
from .background import background

class game:
    def __init__(self):
        self.background = background()
        
        # Name some things we're going to populate in a second

        self.world_size             = [0,0]
        self.player_sprite          = None
        self.ghost_sprite           = None
        self.sleeping_ghost_sprite  = None
        self.ball_sprite            = None

        # Load some resources and build some objects on the 
        # graphics server we're going to use when we render

        self.load_graphics_data()

        # Create our gameplay objects

        self.player = player( self )
        self.ghosts = self.create_ghosts()
        self.ball = ball(self, 16, 14 )

        # Ghost wobble oscillator. (see update)

        self.wobble_index = 0.0   
        self.wobble_speed = 0.09

        # Scores

        self.player_score = 0
        self.ghosts_score = 0

        # We're stroring unit_size here for clarity when referenced
        # later. The explanations in render cover this

        self.unit_size = 16.0   

        # A few settings that only have visual impact

        self.world_magnification = 4
        self.ghost_scale = 1
        self.ball_scale = 1


    
    def create_ghosts(self):
        return [
                    ghost( self, x=7, y=8 ),
                    ghost( self, x=25, y=8 ),
                    ghost( self, x=7, y=25 ),
                    ghost( self, x=25, y=25 )
               ]

    def load_graphics_data(self):

        # Define a dict that will load our sprite / tile data

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

        # Load the tileset (we use this to build and render sprites )
        self.sprite_tileset  = tileset( configuration = configuration, img_path = "tiles/" )

        # Create a sprite renderer. Sprite renderers consume batches of sprites
        # of the same tileset and efficiently renders them. We also need to provide
        # a virtual coordinate system, so the renderer can perform the correct
        # transformation from x:-960+960 y:540+540 to the graphics server 
        # format
        
        CV = centered_view(1920,1080,Y_Axis_Down)
        self.sprite_renderer = sprite_renderer( tileset = self.sprite_tileset, coordinates = CV )

        # Create a bunch of sprite resources.

        self.player_sprite   = sprite( 
                                    sprite_renderer = self.sprite_renderer, 
                                    named_animations = { 
                                                            "up"   : [4, 5, 6, 5 ], 
                                                            "left" : [7, 8, 9, 8 ], 
                                                            "down" : [10, 11,12, 11], 
                                                            "right": [13,14,15, 14]
                                                       }, 
                                    current_animation = "up",
                                    ticks_per_frame = 7 
                                    )

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

        # Load the tilemap

        self.tilemap = tilemap.from_json_file( "map/room.json", "tiles/", coordinates = CV )

        # Set the world size on the game object (self) for reference 

        self.world_size = [ self.tilemap.layers[0]["width"], self.tilemap.layers[0]["height"] ]

    def get_tile(self, x, y):
        return self.tilemap.get_layer_tile(0, x,y)

    def update(self):
        # update the background animation
        self.background.update()


        # Update the sinewave oscillator to wobble the scale of our ghosts
        # between 1.10 and 1.40 
        #
        # 'wobble_index' can be thought of as 't' in sin(t) , we just offset
        # and scale the function.
        #
        self.wobble_index   += self.wobble_speed    # can be thought of as 't' in sin(t)
        self.ghost_scale    = 1.25 + ((sin( self.wobble_index))*0.15) 


        # We tick our sprites so they update their animations
        self.player_sprite.tick()
        self.ghost_sprite.tick()

        # Update the gameplay objects
        self.player.update()
        self.ball.update()
        for ghost in self.ghosts:
            ghost.update()

    def render(self):

        # Rendering the background is straightforward - see class definition 
        # for details.

        self.background.render()

        # We're not managing a 'camera' seperately from the player in this 
        # game; instead using a simple scheme of locking the view to the player.
        #
        # Our player coordinates are in coordinates normalized on 1 unit = 1 tile.
        # However, the default tilemap builder treats 1 unit as 1 'pixel' of the 
        # coordinate  system the tilemap was built with (specified in its constructor)
        #
        # We know our shift in tilespace, and we defined our 'unit_size' on this
        # class as 16 (for easy reference). 
        # 
        # to determine the right location to draw at, we invert the players position 
        # and multiply by the unitsize (16) : 
        #

        self.tilemap.render( -self.player.x*self.unit_size, 
                             -self.player.y*self.unit_size, 
                              self.world_magnification ) 

        # sprite renderers work on batches of render_operations. We're going to create
        # an empty batch and build up the scene we want to render in one quick 
        # blast.

        batch = []


        # First, add the player to the batch...


        batch.append( [ 
                            self.player_sprite,                     # Use the player sprite

                            [ -8.0,-8.0 ] ,                         # Default sprite creation makes 1 pixel = 1 unit, with the
                                                                    # origin of 0,0 in the graphics server VBO. This means our
                                                                    # gemoetry for this 16x16 sprite extends from 0,0 -> 16,16
                                                                    # in vector space. We want to draw our sprite centered on
                                                                    # a coordinate:
                                                                    #
                                                                    # This first parameter transforms the VBO, going from:
                                                                    # A to B in terms of our 'virtual' geometry
                                                                    #
                                                                    #  (a)           (b) -8
                                                                    #                     |
                                                                    #  0->16          -8 -+-8
                                                                    #  |                  |  
                                                                    #  V                 +8
                                                                    #  16


                            1 + self.player.running_power,          # Next we specify the local scale. Use the running power
                                                                    # of the player to make the player look bigger when 
                                                                    # running


                            [0.0,0.0],                              # Next, is the after-scaling transformation. With a 
                                                                    # centered_view coordinate system, 0,0 is dead center
                                                                    # of the screen, so we place it there.
                                                                    #
                                                                    # Pt = (0,0)    Pt = Player transformed to render origin.

                            self.world_magnification                # final entry is post-transformation scale. 
                       ] 
            )

        # Now the ball...

        batch.append( [
                           self.ball_sprite,               
                           [-8.0,-8.0],                             # Same as before,

                           self.ball_scale,                         # Scale here is a class variable for
                                                                    # easy tweaking of game appearance
                                    

                           # Now comes the interesting part. This is a very similar to the transform for 
                           # rendering the tilemap. We take the ball's position and subtract from it the
                           # player's position. As we are locking the player to 0,0 in our centered_view
                           # coordinate system, the difference is the correct offset to render the ball.
                           #
                           #   Bt = (B-P)*u
                           #
                           # This follows for anything you wish to render relative to. An independent
                           # camera could be added to this game, simply by changing the equation:
                           #
                           #    (0,0) = Pt            Pt = player transformed to render origin 
                           #
                           # to:
                           #
                           #    P - C = Pt            C = Camera position
                           #
                           # and 
                           #    
                           #    B - C = Pt
                           #
                           #
                           #

                           [ 
                             (self.ball.x-self.player.x) * self.unit_size, 
                             (self.ball.y-self.player.y) * self.unit_size 
                           ], 
                           
                           self.world_magnification ]
             )


        # Now the ghosts...

        for ghost in self.ghosts:

            if ghost.state == ai_state.sleeping:
                ghost_sprite = self.sleeping_ghost_sprite
            else:
                ghost_sprite = self.ghost_sprite
            batch.append([
                                ghost_sprite, 
                                [ -8.0,-8.0 ],
                                self.ghost_scale, 
                                [ (ghost.fx-self.player.x) * self.unit_size,(ghost.fy-self.player.y)*self.unit_size], 
                                self.world_magnification ])


        # Now blast them off to the graphics server

        self.sprite_renderer.render(batch)
                        
