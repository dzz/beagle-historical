from math import atan2
class player:
    def __init__(self):
        self.vx = 0
        self.vy = 0
        self.x = 0
        self.y = 0
        self.acc = 0
        self.vdecay = 0.99
        self.vr = [0.0,0.0]
        self.r = 0
        self.eng_r = 0
        self.firing = 0
        self.real_acc = 0

    def tick(self, pad,vortex):
        ox = self.x
        oy = self.y

        self.eng_r = 3.14-atan2(pad.left_stick[0],pad.left_stick[1])

        self.acc = pad.triggers[1]+1
        if(self.acc>0):
            self.firing = self.acc
            self.real_acc = 0.99*self.real_acc+(0.01*self.acc*4)
            self.vx = pad.left_stick[0]*self.real_acc
            self.vy = pad.left_stick[1]*self.real_acc
        else:
            self.vx*=self.vdecay
            self.vy*=self.vdecay
            self.firing = 0

        self.x += self.vx
        self.y += self.vy

        [self.x,self.y] = vortex.transform([self.x,self.y])

        a = 0.2
        b = 0.8
        [self.vr[0],self.vr[1]] = [
                                    (self.x-ox)*a + self.vr[0]*b,
                                    (self.y-oy)*a + self.vr[1]*b ]

        self.r = 3.14 - atan2( self.vr[0],self.vr[1] )
