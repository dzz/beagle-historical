from .game import game
import client.ctt2.host_config  as host_config
import audio

import client.system.log as log

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
