WALL_TILE_INDEX = 3

class ball():
    def __init__(self,game,x,y):
        self.x = x
        self.y = y
        self.vx = 0
        self.vy = 0
        self.base_speed = 0.8
        self.decay = 0.98
        self.thrown = False
        self.game = game
    
    def throw(self,vx,vy):
        self.thrown = True
        self.vx = vx
        self.vy = vy

    def update(self):
        if(self.thrown):
            new_x = self.x +  self.vx*self.base_speed
            new_y = self.y +  self.vy*self.base_speed

            if self.game.get_tile( new_x, self.y ) == WALL_TILE_INDEX:
                self.vx*=-1
            else:
                self.x = new_x

            if self.game.get_tile( self.x, new_y ) == WALL_TILE_INDEX:
                self.vy*=-1
            else:
                self.y = new_y

            self.vx *= self.decay
            self.vy *= self.decay


