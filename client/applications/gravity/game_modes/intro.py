from client.gfx.text import render_text
from client.gfx.blend            import blendstate,blendmode
from client.gfx.framebuffer import *
from client.gfx.coordinates import centered_view,Y_Axis_Down
from client.math.helpers import distance
import client.gfx.context as gfxcontext
from random import choice,uniform
from math import sin,cos,atan2
from client.system.gamepad       import get_gamepad
from client.ctt2.assets import assets

class intro_game:
        def __init__(self):
            self.texture = assets.get("intro/planet_background")

        def tick(self, context):
            pass

        def render(self, context):
            pass
