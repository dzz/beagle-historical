from client.gfx.rect             import rect_tile
from client.gfx.tileset          import tileset
from client.gfx.tilemap          import tilemap
from client.system.gamepad       import get_gamepad
from client.gfx.framebuffer      import *
from client.gfx.primitive        import primitive
from client.gfx.primitive        import draw_mode as primitive_draw_mode
from client.gfx.coordinates      import centered_view, Y_Axis_Down
import client.gfx.shaders        as shaders
import client.gfx.context        as gfx_context
import client.system.keyboard    as keyboard

ts = None
tm = None
camera = [0,0]
test_fb = None
test_primitive = None

#bind arrow keys and WASD 

def bindKeyHandlers():
    global camera

    def left():
        camera[0] = camera[0] - 32

    def right():
        camera[0] = camera[0] + 32

    def up():
        camera[1] = camera[1] - 32

    def down():
        camera[1] = camera[1] + 32

    def quit():
        print("goodbye!")
        exit()

    keyboard.register_keypress_handler("escape", quit  )
    keyboard.register_keypress_handler("left",   left  )
    keyboard.register_keypress_handler("right",  right )
    keyboard.register_keypress_handler("up",     up    )
    keyboard.register_keypress_handler("down",   down  )
    keyboard.register_keypress_handler("a",      left  )
    keyboard.register_keypress_handler("d",      right )
    keyboard.register_keypress_handler("w",      up    )
    keyboard.register_keypress_handler("s",      down  )

def init():
    global ts
    global tm
    global test_fb
    global test_primitive

    #set the global clear color to black
    gfx_context.set_clear_color(0.0,0.0,1.0,0.0)


    #Load a tileset via a configuration dict
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
    #we can use this tileset later to render sprites
    ts = tileset( configuration, "roguetiles/Spritesheet/", filtered=True )

    #here's our tilemap - it loads an maintains its own tileset independent of the one we just set up
    tm = tilemap.from_json_file( "json/sample_indoor.json", "roguetiles/Spritesheet/", filtered=True)

    #set a coordinate space the tilemap will use when rendering
    tm.set_coordinates( centered_view( 1920,1080, Y_Axis_Down ) ) 

    bindKeyHandlers()

    #how to create custom geometry
    test_primitive = primitive( primitive_draw_mode.TRI_FAN,
            coords = [
                [  -1.0,  -1.0 ],
                [   1.0,   1.0,],
                [   1.0,   1.0,],
                [  -1.0,   1.0 ]
            ] ,
            uvs = [
                [0.0,0.0],
                [0.0,1.0],
                [1.0,1.0],
                [1.0,0.0]
             ]
            )


def tick():
    global camera
    pad = get_gamepad(0)
    camera[0] += pad.leftStick[0]*32
    camera[1] += pad.leftStick[1]*32

def render():
    global test_fb
    global test_primitive

    gfx_context.clear()

    #render our tilemap
    tm.render(0 - int(camera[0]),0 - int(camera[1]),4)

    #render some sprites
    for gid in range(0,32):
        rect_tile(ts, gid, gid*32, camera[1])

def finalize():
    pass

def configure( configuration ):
    pass
