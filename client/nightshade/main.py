import  host
import  time
from    client.system.gamepad       import get_gamepad
from    math import *
import  hwgfx
from    client.nightshade.scene     import *
from    client.gfx.rect             import rect_solid, rect_brush
import  client.gfx.blend            as blend

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
scene_dir = ""

def init():
    N.scene = loadScene(scene_dir)

target_fps = 60.
tpf = 1000.0/target_fps

def tick():
    pad = get_gamepad(0)

    V.cam[0] += pad.leftStick[0]*32;
    V.cam[1] += pad.leftStick[1]*32;


def render():
    for renderable in N.scene.renderables:
        blend.manual_blend_enter( blend.named_modes[renderable.blendMode] )
        rect_brush( renderable, V )
    blend.manual_blend_exit()

def finalize():
    pass

def configure( configuration ):
    global scene_dir
    scene_dir = configuration["scene_directory"]
    pass
