from client.gfx.rect    import rect_tile
from client.gfx.tileset import tileset
from client.gfx.tilemap import tilemap
from client.system.gamepad       import get_gamepad

ts = None
tm = None
camera = [0,0]

def init():
    global ts
    global tm


    configuration = {
            "image"         : "roguelikeSheet_transparent.png",
            "imageheight"   : 526,
            "imagewidth"    : 968,
            "margin"        : 0,
            "spacing"       : 1,
            "properties"    : {},
            "firstgid"      : 1,
            "tileheight"    : 16,
            "tileproperties" : {} 
            }

    ts = tileset( configuration, "roguetiles/Spritesheet/" )
    tm = tilemap.from_json_file( "json/sample_indoor.json", "roguetiles/Spritesheet/", filtered=False)

def tick():
    global camera
    pad = get_gamepad(0)
    camera[0] += pad.leftStick[0]*32
    camera[1] += pad.leftStick[1]*32

def render():
    tm.render(int(camera[0]),int(camera[1]),2, False)
    tm.render(int(camera[0]),int(camera[1]),1,False)
    for gid in range(0,32):
        rect_tile(ts, gid, gid*32, 0)

def finalize():
    pass

def configure( configuration ):
    pass
