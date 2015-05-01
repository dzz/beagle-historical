from math import floor
from enum import IntEnum
import random

from client.math.helpers import neighbor_coordinates
from client.math.helpers import distance_squared


from .tile_types import types as tiles

class ai_state(IntEnum):
    picking_target_tile = 1
    moving_between_tiles = 2
    sleeping = 3

tile_picking_mode_map = { 0: "target_player", 1: "target_player", 2: "target_ball", 3: "target_random" }
tile_picking_mode_count = 3

class ghost:
    def __init__(self,game,x,y):
        self.x = x                                  # Our base values for position G (x,y)
        self.y = y

        self.starting_x = x
        self.starting_y = y

        self.fx = x                                 # Gf(x,y) == filtered ghost position
        self.fy = y

        self.motion_smoothing = 0.9                 # Coeffecient for the smoothing filter

        self.game = game
        self.state = ai_state.picking_target_tile;  # Initial state

        self.target_x = None                        # Empty Target
        self.target_y = None

        self.speed = 0.1                            # Base Speed


        # Which priority is used when picking a tile, (e.g. target_player, 
        # target_ball )

        self.tile_picking_mode = random.randint(0,tile_picking_mode_count)
        self.max_sleep_time = 60*6

        self.sleep_time = 0                         # How long I've been sleeping

        self.player_charge_x = 0.0                  # Area effect induced by player
        self.player_charge_y = 0.0
        self.player_charge_decay = 0.9              # Loose 10% of player area effect
                                                    # each tick



    def update(self):

        #
        # this is a simple low pass filter used to smooth out the ghost's
        # motion, which otherwise is very angular ( see the moving between
        # tiles state logic )
        #
        # (per sample)
        #
        # filtered_value = old_value * a + new_value * (1 - a)
        #
        #
        # This a type Infinite Impulse Response (IIR) low pass filter
        # 
        # This has the effect of smoothing out the jaggy motion of 
        # self.x and self.y into smoothed self.fx self.fy
        #
        #   (self.x self.y)   (self.fx, self.fy)
        #
        #      +---              .--
        #     /               .*
        #    /              /
        #
        #
        # In practice we use x,y for some logic, and fx,fy for other
        # logic, depending on which vector makes sense for the particular
        # problem domain.

        self.fx = self.fx*self.motion_smoothing+self.x*(1-self.motion_smoothing)
        self.fy = self.fy*self.motion_smoothing+self.y*(1-self.motion_smoothing)

        #
        # the same technique seen elsewhere - we're using the floor function
        # to quantize our position (e.g. [12.45, 10.52] -> [12,10] 
        #
        floor_x = floor(self.x)
        floor_y = floor(self.y)

        # Check if we've been knocked out

        if self.game.get_tile( floor_x, floor_y) is None:
            self.x = self.starting_x;
            self.y = self.starting_y;
        #
        # initial state -- we're going to pick a target x,y, to move towards this tick
        #
        if self.state == ai_state.picking_target_tile:
            #
            # pick a random movmenet mode
            #
            self.tile_picking_mode = tile_picking_mode_map[ random.randint(0,tile_picking_mode_count) ]

            #
            # what follows is each mode mapping to a different sort function, that orders our neighboring
            # tiles accordint to different metrics.
            #
            if self.tile_picking_mode == "target_player":
                sorted_neighbors = sorted( 
                        neighbor_coordinates, 
                        key = lambda n: -1*distance_squared( floor_x+n[0],floor_y+n[1],self.game.player.x,self.game.player.y )
                        )
            elif self.tile_picking_mode== "target_ball":
                sorted_neighbors = sorted( 
                        neighbor_coordinates, 
                        key = lambda n:  -1*distance_squared( floor_x+n[0],floor_y+n[1],self.game.ball.x,self.game.ball.y )
                        )
            elif self.tile_picking_mode== "target_random":
                sorted_neighbors = sorted(neighbor_coordinates, key = lambda x: random.randint(0,8) )

            #
            # Loop through the tiles sorted by our metric, and check each one for 
            # validity. On first valid match, set the target
            #
            for x,y in sorted_neighbors:
                tile = self.game.get_tile( floor_x+x, floor_y+y)
                if tile is not tiles.wall_tile:
                    self.target_x = floor_x + x
                    self.target_y = floor_y + y
                    self.state = ai_state.moving_between_tiles

        #
        # We're moving between tiles this tick
        #
        elif self.state == ai_state.moving_between_tiles: 
            if floor_x == self.target_x and floor_y == self.target_y:
                self.state = ai_state.picking_target_tile
            else:
                #
                # Cheap 45/90 degree movements
                #
                if( floor_x < self.target_x ):
                    self.x += self.speed
                if( floor_x > self.target_x ):
                    self.x -= self.speed
                if( floor_y < self.target_y ):
                    self.y += self.speed
                if( floor_y > self.target_y ):
                    self.y -= self.speed

        #
        # We're sleeping
        #
        elif self.state == ai_state.sleeping:
            #
            # Apply the players 'charge' area effect to 'swoosh' the ghosts
            # along
            #
            self.x += self.player_charge_x
            self.y += self.player_charge_y

            #
            # decay the player charge 
            #
            self.player_charge_x *= self.player_charge_decay
            self.player_charge_y *= self.player_charge_decay

            #
            # ask the game what the world size is
            #
            world_size = self.game.world_size
            if( self.game.get_tile( self.x, self.y) == tiles.empty_space or self.x > world_size[0]  or
                self.x < 0 or self.y > world_size[1]  or self.y < 0):
                #
                # The player knocked us out! Reset the position
                #
                self.x = self.starting_x
                self.y = self.starting_y
                self.fx = self.starting_x
                self.fy = self.starting_y
                self.state = ai_state.picking_target_tile
                self.player_charge_x = 0
                self.player_charge_y = 0
                self.game.player_score += 1
            self.sleep_time = self.sleep_time + 1
            if(self.sleep_time > self.max_sleep_time):
                self.sleep_time = 0
                self.state = ai_state.picking_target_tile


    def put_to_sleep(self):
        self.state = ai_state.sleeping
