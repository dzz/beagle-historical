from math import atan2
class player:
    def __init__(self):
        self.vx = 0
        self.vy = 0
        self.x = 0
        self.y = 0
        self.acc = 0
        self.vdecay = 0.8
        self.r = 0
        self.eng_r = 0
        self.firing = 0

    def tick(self, pad,vortex):
        ox = self.x
        oy = self.y
        self.eng_r = 3.14-atan2(pad.left_stick[0],pad.left_stick[1])
        self.acc = pad.triggers[1]+1
        if(self.acc>0):
            self.firing = self.acc
            self.vx = pad.left_stick[0]*self.acc
            self.vy = pad.left_stick[1]*self.acc
        else:
            self.vx*=self.vdecay
            self.vy*=self.vdecay
            self.firing = 0

        self.x += self.vx
        self.y += self.vy

        [self.x,self.y] = vortex.transform([self.x,self.y])

        self.r = 3.14 - atan2( (self.x-ox),(self.y-oy))
