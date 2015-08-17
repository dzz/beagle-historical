from .game import game
import client.ctt2.host_config  as host_config
import audio

import client.system.log as log

global g
def init():
    global g
    t = audio.clip_create(host_config.get("app_dir") + "audio/level_up.ogg")
    audio.track_play_clip( 0, t )
    g = game()

def tick():
    g.tick()

def render():
    g.render()

def finalize():
    pass

def configure( configuration ):
    pass
