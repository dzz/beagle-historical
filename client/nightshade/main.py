from client.nightshade.scene import *

class Nightshade(object):
    def __init__(self):
        self.scene = None

N = Nightshade()

def init():
    N.scene = loadScene("C:\\ctt2_breezy\\whimsey\\shadeProject\\json\\compiled.json")

def tick():
    pass

def render():
    pass

def finalize():
    pass

