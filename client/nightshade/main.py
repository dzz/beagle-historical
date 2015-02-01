import  time
from    client.system.gamepad       import get_gamepad
from    math import *
import  hwgfx
from    client.nightshade.scene     import *
from    client.gfx.rect             import rect_solid, rect_brush

class view(object):
    def __init__(self):
        self.cam = [0,0]

class Nightshade(object):
    def __init__(self):
        self.scene = None

N = Nightshade()
V = view()
T = 0.0


def init():
    N.scene = loadScene("C:\\ctt2_breezy\\whimsey\\shadeProject\\json\\compiled.json")

target_fps = 60.
tpf = 1.0/target_fps

def tick():
    global T
    a = time.clock() * 0.5
    T   = a
    rad = 125

    pad = get_gamepad(0)

    V.cam[0] += pad.leftStick[0];
    V.cam[1] += pad.leftStick[1];

    pass

def render():
    for renderable in N.scene.renderables:
        rect_brush( renderable, V )

    delta = T + tpf - time.clock()
    if delta>0:
        time.sleep(delta)

def finalize():
    pass

