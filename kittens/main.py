import host
import hwgfx
import gfx
import ui_area
import ui_window
import kittens.application.main

mouse_focused_area = None
caret_handler = None

ticks = 0

__clickpos = [0,0]
__mpos = [0,0]

def init():
    kittens.application.main.init()

def finalize():
    pass

def tick():
    gfx.blend_enter( gfx.BLENDMODE_OVER )
    render()
    gfx.blend_exit()

def _get_mf_area():
    return mouse_focused_area

def render():
    global __mpos
    global __clickpos
    ui_area.find_ui_area(__mpos[0],__mpos[1])
    for area in ui_area.order_areas():
        for renderer in area.renderers:
            renderer.render(area)
    
def calculate_mouse_position(area,x,y):
    xt = min(max( 0, x - area.r[0] ),area.r[2])
    yt = min(max( 0, y - area.r[1] ),area.r[3])
    return [xt,yt] 

def dispatch_mouseup(button,x,y):
    global mouse_focused_area
    if mouse_focused_area is not None:
        m_pos = calculate_mouse_position(mouse_focused_area,x,y);
        mouse_focused_area.rcv_mouse_button(button,m_pos[0],m_pos[1], down = False)
        mouse_focused_area.active = False
        mouse_focused_area = None

def dispatch_mousedown(button,x,y):
    global mouse_focused_area
    __clickpos[0] = x
    __clickpos[1] = y

    area = ui_area.find_ui_area(x,y)
    if area is not None:
        m_pos = calculate_mouse_position(area,x,y);
        area.rcv_mouse_button(button,m_pos[0],m_pos[1], down = True)
        area.active = True
        mouse_focused_area = area


def dispatch_mousemotion(x,y):
    global __mpos
    global mouse_focused_area
    __mpos = [x,y]
    ui_area.set_absolute_mpos([x,y])
    if mouse_focused_area is None:
        area = ui_area.find_ui_area(x,y)
        if area is not None:
            area.set_m(calculate_mouse_position(area,x,y))
    else:
        mouse_focused_area.set_m(calculate_mouse_position(mouse_focused_area,x,y))

def dispatch_key(key,down):
    global caret_handler
    if(caret_handler == None):
        dispatch_root_keybindings(key,down)
        if(mouse_focused_area is not None):
            area.rcv_key(key,down)
        return
    caret_handler.rcv_key(key,down)

def dispatch_root_keybindings(key,down):
    pass

def caret_release():
    global caret_handler
    caret_handler = None

