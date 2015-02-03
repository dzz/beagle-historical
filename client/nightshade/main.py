import  host
import  time
from    client.system.gamepad       import get_gamepad
from    math import *
import  hwgfx
from    client.nightshade.scene     import *
from    client.gfx.rect             import rect_solid, rect_brush

class view(object):
    def __init__(self):
        self.cam = [0,0]
        self.screen_center = [ 1920/2, 1080/2 ]

class Nightshade(object):
    def __init__(self):
        self.scene = None

N = Nightshade()
V = view()
T = 0.0


def init():
    N.scene = loadScene("C:\\ctt2_breezy\\whimsey\\shadeProject\\json\\compiled.json")

target_fps = 60.
tpf = 1000.0/target_fps

def tick():
    pad = get_gamepad(0)

    V.cam[0] += pad.leftStick[0]*32;
    V.cam[1] += pad.leftStick[1]*32;


def render():
    for renderable in N.scene.renderables:
        rect_brush( renderable, V )

def finalize():
    pass

