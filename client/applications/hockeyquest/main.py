import client.gfx.text as text
import client.ctt2.host_config  as host_config

from client.applications.hockeyquest.text_emu import get_default_scr
from client.applications.hockeyquest.root_game import get_game

game = None

def init():
    global game
    game = get_game( get_default_scr() )
    host_config.set_config("render_status",False)
    game.init()

def tick():
    game.tick()

def render():
    game.render()

def finalize():
    game.finalize()

def configure( configuration ):
    pass
