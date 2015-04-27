from math import floor
import random

from client.math.helpers import neighbor_coordinates
from client.math.helpers import distance_squared

from .tile_types import *

#ghost states
PICKING_TILE = 1
MOVING_TILES = 2
SLEEPING = 3

class ghost:
    def __init__(self,game,x,y):
        self.x = x
        self.y = y

        self.fx = x  #we're going to filter the x,y coords as the AI updates them to smooth out the motion and make it more natural
        self.fy = y

        self.motion_smoothing = 0.9

        self.game = game
        self.state = PICKING_TILE
        self.target_x = None
        self.target_y = None
        self.speed = 0.1
        self.movement_mode = 0
        self.max_sleep_time = 60*3
        self.sleep_time = 0

    def update(self):

        self.fx = self.fx*self.motion_smoothing+self.x*(1-self.motion_smoothing)
        self.fy = self.fy*self.motion_smoothing+self.y*(1-self.motion_smoothing)

        floor_x = floor(self.x)
        floor_y = floor(self.y)
        if self.state == PICKING_TILE:

            #this will cycle through 0,1,2 everytime the picking_tile state is hit the behaviour is,
            #the ghost will pick random title 2/3 times, but 1/3 times will pick a tile that moves 
            #the ghost closer to the player

            self.movement_mode = (self.movement_mode+1)%3
            
            if self.movement_mode==0:
                #if movement_mode == 0, then sort the potential targets by how close they are to the player
                #... i'm not exactly sure why the y axis needs to be inverted here! 
                sorted_neighbors = sorted( 
                        neighbor_coordinates, 
                        key = lambda n: distance_squared( floor_x+n[0],floor_y-n[1],self.game.player.x,self.game.player.y )
                        )
            else:
                #okay, otherwise, just randomly sort our candidate titles
                sorted_neighbors = random.sample( neighbor_coordinates, len(neighbor_coordinates) )

            for x,y in sorted_neighbors:
                tile = self.game.get_tile( floor_x+x, floor_y+y)
                if tile is not WALL_TILE_INDEX:
                    self.target_x = floor_x + x
                    self.target_y = floor_y + y
                    self.state = MOVING_TILES
        elif self.state == MOVING_TILES: 
            if floor_x == self.target_x and floor_y == self.target_y:
                self.state = PICKING_TILE
            else:
                if( floor_x < self.target_x ):
                    self.x += self.speed
                if( floor_x > self.target_x ):
                    self.x -= self.speed
                if( floor_y < self.target_y ):
                    self.y += self.speed
                if( floor_y > self.target_y ):
                    self.y -= self.speed

        elif self.state == SLEEPING:
            self.sleep_time = self.sleep_time + 1
            if(self.sleep_time > self.max_sleep_time):
                self.sleep_time = 0
                self.state = PICKING_TILE


    def put_to_sleep(self):
        self.state = SLEEPING
