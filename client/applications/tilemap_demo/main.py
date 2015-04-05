from client.gfx.rect    import rect_tile
from client.gfx.tileset import tileset
from client.gfx.tilemap import tilemap

ts = None
tm = None

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
    pass

def render():
    tm.render(0,0,2)
    for gid in range(0,32):
        rect_tile(ts, gid, gid*32, 0)

def finalize():
    pass

def configure( configuration ):
    pass
