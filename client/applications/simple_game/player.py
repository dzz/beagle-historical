from math                        import atan2, pi, floor
from client.system.gamepad       import get_gamepad
from client.math.helpers         import rectangles_intersect, neighbor_coordinates, distance_squared
from .tile_types                 import types as tiles
from .ghost                      import ai_state


class player:
    def __init__(self, game):

        # we're going to define ourselves as starting at the middle of
        # the tile map. In the domain of players, ghosts and balls,
        # 1 unit is 1 tile. Our tilemap is 32x32 tiles, so we set ourselves
        # to 16,16

        self.x = 16.0                           # [16,16] = P  is our current position
        self.y = 16.0
        self.starting_x = 16.0                  # Keep track of Ps as well
        self.starting_y = 16.0

        self.player_speed = 0.17                # How much does stick motion move the player?


        # Parameters on some of the gameplay mechanics

        self.holding_ball_speed_penalty = 0.07

        self.running_power = 0;                     # State variable for "amount that we are running" 
                                                    #
                                                    # R = 0 in initial state

        self.running_cutoff = 0.3;                  # Rc = 0.3
                                                    #
                                                    # if R < Rc then R is reset to 0.

        self.running_power_decay = 0.97;            # Rd = 0.97
                                                    #
                                                    # if R > Rc then next R is R*Rd 
                                                    #
                                                    # This is equivalent to losing 3% of
                                                    # its value every frame

        self.running_power_speed_boost = 0.05       # Rsb = 0.05 
                                                    #
                                                    # how much running boosts speed. ( Rsb*R ) = total boost

        self.animation_threshold = 0.05             # How fast do we have to be moving to trigger the animation


        # This is the Hit Box Size / 2, since /2 is more convenient for the
        # calculations. We make the hitbox slightly smaller than a full unit, because
        # otherwise it required utmost precision and epsilon juggling to fit
        # through a 1 tile cooridor.
        #
        self.player_collision_size_over2 = 0.45

        # For certain collisions, we will model the player as a circle. For these
        # calculations, it is convenient to have the the radius already squared. 
        # The reason is, we can determine if a point P is inside a circle C via the
        # distance function:
        #             _______________________
        #     ___    / 
        #  D =   \  /  (Px-Cx)^2 + (Py-Cy)^2
        #         \/
        #
        # If D is less than the radius (r) of C, then the point is inside the circle.
        #
        # However, we can eliminate the expensive square root:
        #
        #  D^2 = (Px-Cx)^2 + (Py-Cy)^2
        #
        # And compare instead D^2 to r^2
        #
        #
        self.player_collision_size_squared = 0.45*0.45

        # we say when the player is running, he can 'charge' ghosts with his
        # energy and sweep them along with him. We do point in circle
        # checks for this as well, hence the _squared 
        #
        self.player_charge_zone_squared = 2


        self.direction = "up" 
        self.animation_states = [ "up", "left", "down", "right" ] 
        self.game = game
        self.has_ball = False


    def update(self):

        def update_animation(sprite,velocity_x,velocity_y):
            #
            # the accurate method to compute the length of a vector is to use
            # the D=Sqrt(dx^2+dy^2) function, however; in this case we're just
            # triggering an animation, dont care about accuracy, and can get away with
            # just an absolute function. Try plotting plotting abs(x),abs(y) vs 
            # Sqrt(x^2+y^2) to see the how they are different, but similar functions
            #

            cheap_speed = abs( velocity_x) + abs ( velocity_y )

            if( cheap_speed > self.animation_threshold ):
                sprite.set_animation_enabled(True)
            else:
                sprite.set_animation_enabled(False)

            if cheap_speed > 0 :
                # 
                # atan2(x,y) will give us the angle of the vector in radians (-pi <-> pi)
                #
                # we scale, clamp and quantize the value. The clamping might
                # not strictly be necessary, but it makes me feel safe.
                #
                angle = ( (atan2( velocity_x, velocity_y ) + pi ) / (2*pi) ) * 4
                angle = max(0,int(angle))
                angle = min(angle,3)

                #
                # did the state change? If so, change the animation for
                # the sprite
                #

                if( self.animation_states[angle] != self.direction ):
                    self.direction = self.animation_states[angle]
                    sprite.select_animation( self.direction )


        def handle_run(self, pad, velocity_x, velocity_y):
            #
            # Run slower, until decays below the cutoff, and then back to normal walking speed.
            #
            # if R(0) > Rc, R(1) = R * Rd ; if R(0) < Rc, R(1) = 0;
            #
            
            if self.running_power > self.running_cutoff:
                self.running_power = self.running_power * self.running_power_decay
            else:
                self.running_power = 0.0

            #
            # If the left trigger is being pulled, and our guy is below the
            # run cutoff, we force him to drop the ball and then set his running
            # power (R) to 100%
            #
            if( pad.triggers[0] > 0.0 and (self.running_power< self.running_cutoff) ):
                self.has_ball = False
                self.running_power = 1.0
            
            #
            # now, iterate on ghosts
            #
            #
            for ghost in self.game.ghosts:
                #
                # if we're running
                #
                if( self.running_power > self.running_cutoff ):
                    #
                    # and the ghost is sleeping
                    #
                    if ghost.state == ai_state.sleeping:
                        #
                        # check if the center of the ghost is within the players circle
                        #
                        if distance_squared( self.x, self.y, ghost.fx, ghost.fy ) < self.player_charge_zone_squared:
                            #
                            # Then charge the ghost with some area-influence velocity
                            #
                            ghost.player_charge_x = velocity_x
                            ghost.player_charge_y = velocity_y


        def check_for_ghost_attacks(self):
                #
                # Check if a ghost  hits a player. We only let the ghost 'kill' 
                # the player when he's making a decision about where to move next, 
                # to make it a bit easier on the player
                #

                for ghost in self.game.ghosts:
                    if ghost.state == ai_state.picking_target_tile:
                        if distance_squared( self.x, self.y, ghost.x, ghost.y ) < self.player_collision_size_squared:
                            self.x = self.starting_x
                            self.y = self.starting_y
                            self.game.ghosts_score +=1
                            for ghost in self.game.ghosts:
                                ghost.x = ghost.starting_x
                                ghost.y = ghost.starting_y

        def handle_ball(self, pad):
            #
            # If our guy has the ball
            #
            if(self.has_ball):
                #
                # Swivel the ball's position around the player oriented by the
                # right stick
                #
                self.game.ball.x = self.x + (pad.right_stick[0]*0.5)
                self.game.ball.y = self.y + (pad.right_stick[1]*0.5)
                if( pad.triggers[1] > 0.0):
                    self.game.ball.throw( pad.right_stick[0], pad.right_stick[1] )
                    self.has_ball = False
            elif(self.running_power < self.running_cutoff ):
                #
                # Collision Detection: Walking Player vs. Ball on Floor (for pickup)
                #
                # Technique: Quantize both the ball and player positions (i.e. drop
                #            the decimal portion 1.x -> 1.0 ) and do a simple comparison.
                #
                #
                floor_player_x = floor(self.x)
                floor_player_y = floor(self.y)
                floor_ball_x = floor(self.game.ball.x)
                floor_ball_y = floor(self.game.ball.y)
                #
                # Are the player and ball on the same tile?
                #
                if floor_player_x == floor_ball_x and floor_player_y == floor_ball_y and (pad.triggers[1]<0):
                    #
                    # pickup the ball
                    #
                    self.has_ball = True


        def get_collision_rect(self,x,y):
            #
            # this is a simple helper function that checks the game tiles to see if
            # a tile is a non-walkable tile, and if so, return a rectangle that 
            # represents that tile.
            #
            if self.game.get_tile( x,y)  in [ tiles.wall_tile, tiles.empty_space ]:
                return [ x, y, x+1, y+1 ] 
            return None

        def get_collidable_rectangles_in_neighborhood(self):
            #
            # query the world for collidable 1x1 rectangles represnting
            # tiles.
            #
            collision_rects = []                                    

            #
            # neighbor_coordinates (from client.math.helpers) is a list
            # of of neighbors e.g. every permutation of (+1,+/-1)
            #
            # we 'iterate the space', and accumulate rectangles 
            #
            for x,y in neighbor_coordinates:
                collision_rect = get_collision_rect( self, int(self.x + x), int(self.y + y) )
                if( collision_rect is not None):
                    collision_rects.append(collision_rect)

            return collision_rects

        def check_for_map_collisions(self, velocity_x, velocity_y):
            #
            # Algorithm Description: 
            #                  
            #                 
            #      B.      T .
            #               +--------+    
            #      P.+--+  A|. W     |
            #        |  |   |        |
            #        +--+   +--------+
            #
            #
            #  Player Box P is attempting to move to position T. What happens?
            #  (and what should happen?)
            #  
            #  First we try moving P along the X-axis to to A = (Tx,Py)
            #  (If this task had succeeded, we would have updated the X value
            #   on the player object )
            #
            #  A puts P inside wall 'W', the test fails. 
            #
            #  Now we try moving P along the Y-axis to B = (Px,Ty)
            #
            #  This test succeeds so we update the Y value on our player
            #  to the new position.
            #
            #                  
            #               
            #                 
            #      P.+--+  
            #        |  |   +--------+    
            #        +--+  A|. W     |
            #           h---*g       |
            #               +--------+
            #
            #
            # This could be made more accurate, allowing P to get closer to
            # goal T by minimizing the distance to the wall A; calculating
            # the intersection between h = Px+Pw and g = Wx
            #
            # 
            # In practice, at 60Hz timing resolution and floating point
            # gamepad input the cheaper method works out okay.
            #
            #

            # start by getting rectangles representing the tiles in our
            # neighborhood
            #
            collision_rects = get_collidable_rectangles_in_neighborhood(self)


            #
            # set up new_x, new_y vars for our test rectangle
            #
            new_x = self.x + velocity_x 
            new_y = self.y + velocity_y

            # use hitbox size from class
            sz = self.player_collision_size_over2

            #
            # set up a hit box:
            #
            #          -sz
            #
            #      -sz  P  +sz
            #
            #          +sz
            #

            # slide P along the X axis, for first test
            horizontal_player_rect = [ new_x-sz, self.y-sz, new_x+sz, self.y+sz ]

            horizontal_collision = False
            for collision_rect in collision_rects:
                if rectangles_intersect( horizontal_player_rect, collision_rect ):
                    horizontal_collision = True
                    break
            if not horizontal_collision:
                #
                # if no collision, set the 'real' X to be the new value
                #
                self.x = new_x

            #
            # ditto, for Y axis (slide up, set if we can slide)
            #
            vertical_player_rect = [ self.x-sz, new_y-sz, self.x+sz, new_y+sz ]

            vertical_collision = False
            for collision_rect in collision_rects:
                if rectangles_intersect( vertical_player_rect, collision_rect ):
                    vertical_collision = True
                    break

            if not vertical_collision:
                self.y = new_y


        #
        # update(self): begins here
        #


        # get player 1's gamepad

        pad         = get_gamepad(0)



        # If player has has the ball, apply the penalty on speed. 
        if(self.has_ball):
            modified_player_speed = self.player_speed - self.holding_ball_speed_penalty
        else:
            modified_player_speed = self.player_speed

        # The modified_player_speed variable refers to the percentage of stick motion that gets
        # converted into velocity in units per-tick  (e.g. 0.01 units per 1/60th of a second )
        # 
        # V = MPS*(Rp*Rpsb)
        # 
        velocity_x  = pad.left_stick[0]*(modified_player_speed+(self.running_power*self.running_power_speed_boost))
        velocity_y  = pad.left_stick[1]*(modified_player_speed+(self.running_power*self.running_power_speed_boost))

        update_animation( self.game.player_sprite, velocity_x, velocity_y)

        check_for_map_collisions( self, velocity_x, velocity_y )
        handle_ball(self,pad)
        handle_run(self,pad, velocity_x, velocity_y)
        check_for_ghost_attacks(self)


