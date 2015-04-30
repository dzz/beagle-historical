from .tile_types    import types as tiles
from .ghost         import ai_state 

class ball():
    def __init__(self,game,x,y):
        self.x = x
        self.y = y
        self.vx = 0
        self.vy = 0
        self.starting_x = x
        self.starting_y = y
        self.base_speed = 0.85
        self.decay = 0.93
        self.thrown = False
        self.stop_threshold = 0.001
        self.game = game
        self.collision_circle_squared = 0.5
    
    def throw(self,vx,vy):
        self.thrown = True
        self.vx = vx
        self.vy = vy

    def update(self):
        if(self.thrown):
            new_x = self.x +  self.vx*self.base_speed
            new_y = self.y +  self.vy*self.base_speed

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

            self.vx *= self.decay
            self.vy *= self.decay


            if( abs(self.vx+self.vy) > self.stop_threshold):
                for ghost in self.game.ghosts:
                    if ghost.state == ai_state.sleeping:
                        continue
                    dx = ghost.fx - self.x
                    dy = ghost.fy - self.y
                    md = dx*dx+dy*dy
                    if md < self.collision_circle_squared:
                        ghost.put_to_sleep()
                        ghost.player_charge_x = self.vx
                        ghost.player_charge_y = self.vy
                        self.vx *= 0.5
                        self.vy *= 0.5

