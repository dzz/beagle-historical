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
        self.fuzzy_firing_state = 0
        self.real_acc = 0

    def tick(self, pad,vortex,music_system):
        ox = self.x
        oy = self.y

        self.eng_r = 3.14-atan2(pad.left_stick[0],pad.left_stick[1])

        self.acc = pad.triggers[1]+1
        fuzzy_firing_impulse = 0.0
        if(self.acc>0):
            fuzzy_firing_impulse = 1.0
            self.firing = self.acc
            self.real_acc = (0.93*self.real_acc)+(0.07*self.acc*1)
            self.vx += (pad.left_stick[0]*(self.real_acc*self.real_acc))/8;
            self.vy += (pad.left_stick[1]*(self.real_acc*self.real_acc))/8;
        else:
            self.vx*=self.vdecay
            self.vy*=self.vdecay
            self.firing = 0

        self.fuzzy_firing_state = (0.7*self.fuzzy_firing_state)+(0.3*fuzzy_firing_impulse)
        music_system.track_volume("DrumEffects",self.fuzzy_firing_state)
        self.x += self.vx
        self.y += self.vy

        [self.x,self.y] = vortex.transform([self.x,self.y])

        a = 0.2
        b = 0.8
        [self.vr[0],self.vr[1]] = [
                                    (self.x-ox)*a + self.vr[0]*b,
                                    (self.y-oy)*a + self.vr[1]*b ]

        self.r = 3.14 - atan2( self.vr[0]*0.25+pad.left_stick[0]*0.75,self.vr[1]*0.25+pad.left_stick[1]*0.75 )
