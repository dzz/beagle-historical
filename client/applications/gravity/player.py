from math import atan2
class player:
    def __init__(self):
        self.vx = 0
        self.vy = 0
        self.x = 0
        self.y = 0
        self.acc = 0
        self.vdecay = 0.8

    def tick(self, pad,vortex):
        ox = self.x
        oy = self.y
        self.acc = pad.triggers[1]+1
        if(self.acc>0):
            self.vx = pad.left_stick[0]*self.acc
            self.vy = pad.left_stick[1]*self.acc
        else:
            self.vx*=self.vdecay
            self.vy*=self.vdecay

        self.x += self.vx
        self.y += self.vy

        [self.x,self.y] = vortex.transform([self.x,self.y])

        self.r = 3.14 - atan2( (self.x-ox)*0.2+(pad.left_stick[0]*0.8),(self.y-oy)*0.2+(pad.left_stick[1]*0.8))
