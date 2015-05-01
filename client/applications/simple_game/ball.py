from .tile_types    import types as tiles
from .ghost         import ai_state 

class ball():
    def __init__(self,game,x,y):

        self.game = game
        #
        #         we're going to model the ball as a simple 
        #         x,y vector (P), with velocity vx,vy (V)
        #    
        #         
        self.x = x
        self.y = y
        self.vx = 0
        self.vy = 0
        self.starting_x = x
        self.starting_y = y
        self.base_speed = 0.85      # We're going to add an extra term
                                    # to our motion calculation to allow for
                                    # easy manipulation of gameplay behaviour
                                    #
                                    # P = Position, V = Velocity, S = Base Speed
                                    #
                                    # P(1) = P(0) + V * S
                                    #

        self.decay = 0.93           # We see this pattern alot, applying decay
                                    # as type of IIR:
                                    #
                                    #
                                    # V(1) = V(0) * D


        self.thrown = False         # When the player throws the ball, this will 
                                    # set to True

        self.stop_threshold = 0.001 # This threshold is used to determine when the 
                                    # ball is stopped. If the function:
                                    #
                                    # f(vx,vy) = abs(vx) + abs(vy) 
                                    #
                                    # drops below this threshold, the ball is considered
                                    # "stopped" and self.thrown is set to False, and
                                    # the ball can no longer hit enemies


        # using circle collision vs. ghosts, in a cheap manner (if the 
        # ghost's center point is inside the ball's sphere, the ghost will 
        # be considered 'hit' )

        self.collision_circle_squared = 0.5
    
    def throw(self,vx,vy):
        self.thrown = True
        self.vx = vx
        self.vy = vy

    def update(self):
        if(self.thrown):

            # If our current position is P(0) then the next position 
            # P(1) is calculated like so:
            # 
            # P(1) = P(0) + V * S

            new_x = self.x +  self.vx*self.base_speed
            new_y = self.y +  self.vy*self.base_speed

            # similar to how we calculate player vs. map collisions, 
            # here we first slide along x, and then along y. The 
            # difference here is that, if in sliding the ball along the
            # X axis places the ball into a wall tile, we invert 
            # self.vx to make it reverse direction; the same follows
            # for Y axis motion: test, if collision, invert velocity.
            #
            
            if self.game.get_tile( new_x, self.y ) == tiles.wall_tile:
                self.vx*=-1
            else:
                self.x = new_x

            if self.game.get_tile( self.x, new_y ) == tiles.wall_tile:
                self.vy*=-1
            else:
                self.y = new_y


            if(self.game.player.has_ball is False): 
                if( abs(self.vx+self.vy) < self.stop_threshold):
                    if self.game.get_tile(self.x,self.y) == tiles.empty_space:
                            self.x = self.starting_x;
                            self.y = self.starting_y;

            # Here we implement:
            #
            # V(1) = V(0) * D
            #

            self.vx *= self.decay
            self.vy *= self.decay


            if( abs(self.vx) + abs(self.vy) ) > self.stop_threshold):
                #
                # If the ball is active...
                #
                for ghost in self.game.ghosts:
                    #
                    # ...and the ghost is not sleeping...
                    #
                    if ghost.state == ai_state.sleeping:
                        continue
                    #
                    # check for a collision...
                    #
                    dx = ghost.fx - self.x
                    dy = ghost.fy - self.y
                    md = dx*dx+dy*dy
                    if md < self.collision_circle_squared:
                        #
                        # .. and if there was a collision, 
                        #    we put the ghost to sleep.
                        #
                        ghost.put_to_sleep()
                        ghost.player_charge_x = self.vx
                        ghost.player_charge_y = self.vy
                        #
                        # .. and for extra credit, we cut the ball's
                        #    velocity in half.
                        #
                        self.vx *= 0.5
                        self.vy *= 0.5

