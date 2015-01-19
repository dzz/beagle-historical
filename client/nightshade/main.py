import  hwgfx
from    client.nightshade.scene     import *
from    client.gfx.rect             import rect_solid

class Nightshade(object):
    def __init__(self):
        self.scene = None

N = Nightshade()


def init():
    N.scene = loadScene("C:\\ctt2_breezy\\whimsey\\shadeProject\\json\\compiled.json")

def tick():
    pass

def render():
    for renderable in N.scene.renderables:
        rect_solid( renderable.r, renderable.colour )

def finalize():
    pass

