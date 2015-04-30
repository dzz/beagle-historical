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


class ghost:
    def __init__(self,game,x,y):
        self.x = x
        self.y = y

        self.fx = x  #we're going to filter the x,y coords as the AI updates them to smooth out the motion and make it more natural
        self.fy = y

        self.motion_smoothing = 0.9

        self.game = game
        self.state = ai_state.picking_target_tile;
        self.target_x = None
        self.target_y = None
        self.speed = 0.1
        self.movement_mode = random.randint(0,3)
        self.max_sleep_time = 60*6
        self.sleep_time = 0

        self.player_charge_x = 0.0
        self.player_charge_y = 0.0
        self.player_charge_decay = 0.9

        self.starting_x = x
        self.starting_y = y


    def update(self):

        self.fx = self.fx*self.motion_smoothing+self.x*(1-self.motion_smoothing)
        self.fy = self.fy*self.motion_smoothing+self.y*(1-self.motion_smoothing)

        floor_x = floor(self.x)
        floor_y = floor(self.y)

        # did the player kick us out of the arena?
        if self.game.get_tile( floor_x, floor_y) is None:
            self.x = self.starting_x;
            self.y = self.starting_y;

        if self.state == ai_state.picking_target_tile:

            #this will cycle through 0,1,2 everytime the picking_tile state is hit the behaviour is,
            #the ghost will pick random title 2/3 times, but 1/3 times will pick a tile that moves 
            #the ghost closer to the player

            self.movement_mode = random.randint(0,4)
            
            if self.movement_mode ==0 or self.movement_mode == 3:
                #if movement_mode == 0 or 1 then sort the potential targets by how close they are to the player
                #... i'm not exactly sure why the y axis needs to be inverted here! 
                sorted_neighbors = sorted( 
                        neighbor_coordinates, 
                        key = lambda n: 0.0 - distance_squared( floor_x+n[0],floor_y+n[1],self.game.player.x,self.game.player.y )
                        )

            elif self.movement_mode==1:
                #if movement_mode == 1, sort by distance to ball to mess with the player
                sorted_neighbors = sorted( 
                        neighbor_coordinates, 
                        key = lambda n:  -1* distance_squared( floor_x+n[0],floor_y+n[1],self.game.ball.x,self.game.ball.y )
                        )
            elif self.movement_mode==2:
                sorted_neighbors = sorted(neighbor_coordinates, key = lambda x: random.randint(0,8) )
            else:
                sorted_neighbors = list(neighbor_coordinates)

            
            for x,y in sorted_neighbors:
                tile = self.game.get_tile( floor_x+x, floor_y+y)
                if tile is not tiles.wall_tile:
                    self.target_x = floor_x + x
                    self.target_y = floor_y + y
                    self.state = ai_state.moving_between_tiles

        elif self.state == ai_state.moving_between_tiles: 
            if floor_x == self.target_x and floor_y == self.target_y:
                self.state = ai_state.picking_target_tile
            else:
                if( floor_x < self.target_x ):
                    self.x += self.speed
                if( floor_x > self.target_x ):
                    self.x -= self.speed
                if( floor_y < self.target_y ):
                    self.y += self.speed
                if( floor_y > self.target_y ):
                    self.y -= self.speed

        elif self.state == ai_state.sleeping:
            self.x +=self.player_charge_x
            self.y += self.player_charge_y

            self.player_charge_x *= self.player_charge_decay
            self.player_charge_y *= self.player_charge_decay

            world_size = self.game.world_size
            if( self.game.get_tile( self.x, self.y) == tiles.empty_space or self.x > world_size[0]  or
                self.x < 0 or self.y > world_size[1]  or self.y < 0):
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
