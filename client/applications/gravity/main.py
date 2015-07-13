from .background import background
from .vortex import vortex
from client.system.gamepad       import get_gamepad

class game:
    def __init__(self):
       self.background = background()
       self.vortex = vortex()

    def tick(self):
       pad         = get_gamepad(0)
       #print( pad.left_stick); 
       self.background.update()
       self.background.x += pad.left_stick[0];
       self.background.y += pad.left_stick[1];
       [self.background.x, self.background.y ] = self.vortex.transform( [ self.background.x, self.background.y ])

    def render(self):
        self.background.render()


global g
def init():
    global g
    g = game()

def tick():
    g.tick()

def render():
    g.render()

def finalize():
    pass

def configure( configuration ):
    pass
