import host
import hwgfx
import configparser
import client.system.gamepad as gamepad

from client.ctt2.mouse_focus    import mouse_focused_area
from client.ctt2.status import render_status
from client.ctt2.status import set_status



import client.ctt2.host_config  as host_config
import client.apps
import client.ui.areas          as ui_area
import client.gfx.blend         as blend
import client.ctt2.caret        as caret
import gc

__clickpos  = [0,0]
__mpos      = [0,0]


global app 

def init():
    def bool(v):
        if v is False: 
            return False
        return v.lower() in ("true","1")

    global app   

    config = configparser.ConfigParser()
    config.read("client/application.ini")
    app_name = config["APPLICATION"]["name"]
    controller_enabled = bool( config["APPLICATION"]["controller_enabled"] );
    app = client.apps.get_app(app_name) 
    app.controller_enabled = controller_enabled
    app.init()
    set_status("initialized application:" + app_name)
    if(app.controller_enabled):
        print("app requesting controller input")
        gamepad.init()

def finalize():
    global app
    app.finalize()

def tick():
    global app
    if(app.controller_enabled):
        gamepad.tick()
    app.tick()
    render()
    gc.collect()

def render():
    with blend.state(blend.mode_over):
        for area in ui_area.order_areas():
            for renderer in area.renderers:
                renderer.render(area)
        app.render()
        if host_config.get_config("render_status"):
            render_status()
    
def _get_mf_area():
    return mouse_focused_area

def calculate_mouse_position(area,x,y):
    xt = min(max( 0, x - area.r[0] ),area.r[2])
    yt = min(max( 0, y - area.r[1] ),area.r[3])
    return [xt,yt] 

SIGNAL_HANDLED      = True
SIGNAL_DISCARDED    = False

def dispatch_mouseup(button,x,y):
    global mouse_focused_area
    if mouse_focused_area is not None:
        m_pos = calculate_mouse_position(mouse_focused_area,x,y);
        mouse_focused_area.rcv_mouse_button(button,m_pos[0],m_pos[1], down = False)
        mouse_focused_area = None
        return SIGNAL_HANDLED
    return SIGNAL_DISCARDED

def dispatch_mousedown(button,x,y):
    global mouse_focused_area
    __clickpos[0] = x
    __clickpos[1] = y

    area = ui_area.find_ui_area(x,y)
    if area is not None:
        m_pos = calculate_mouse_position(area,x,y);
        area.rcv_mouse_button(button,m_pos[0],m_pos[1], down = True)
        mouse_focused_area = area
        return SIGNAL_HANDLED
    return SIGNAL_DISCARDED

def dispatch_mousemotion(x,y):
    global __mpos
    global mouse_focused_area
    __mpos = [x,y]
    ui_area.set_absolute_mpos([x,y])
    if mouse_focused_area is None:
        area = ui_area.find_ui_area(x,y)
        if area is not None:
            area.rcv_mousemotion(*(calculate_mouse_position(area,x,y)))
            return SIGNAL_HANDLED
    else:
        mouse_focused_area.rcv_mousemotion(*(calculate_mouse_position(mouse_focused_area,x,y)))
        return SIGNAL_HANDLED
    return SIGNAL_DISCARDED

def dispatch_key(key,down):
    KEY_ESCAPE = 33
    caret_target = caret.get_caret()
    if down:
        if key == KEY_ESCAPE:
            caret.drop()
            return

    if caret_target is not None:
        caret_target.rcv_key(key,down)


def dispatch_root_keybindings(key,down):
    pass

def dispatch_text(text):
    caret_target = caret.get_caret()
    if caret_target is not None:
        caret_target.rcv_text(text)
    return
