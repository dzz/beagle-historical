from client.gfx.tileset import tileset

def init():

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
    pass

def tick():
    pass

def render():
    pass

def finalize():
    pass

def configure( configuration ):
    pass
