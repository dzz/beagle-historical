import host
import hwgfx
import client.ui.areas as ui_area
import client.ui.window as ui_window
import client.app.main
import client.gfx.blend as blend

mouse_focused_area = None
caret_handler = None

__clickpos  = [0,0]
__mpos      = [0,0]

def init():
    client.app.main.init()

def finalize():
    client.app.main.finalize()

def tick():
    with blend.state(blend.mode_over):
        render()

def render():
    global __mpos
    global __clickpos
    ui_area.find_ui_area(__mpos[0],__mpos[1])
    for area in ui_area.order_areas():
        for renderer in area.renderers:
            renderer.render(area)
    
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
        mouse_focused_area.active = False
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
        area.active = True
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
            area.set_m(calculate_mouse_position(area,x,y))
            return SIGNAL_HANDLED
    else:
        mouse_focused_area.set_m(calculate_mouse_position(mouse_focused_area,x,y))
        return SIGNAL_HANDLED
    return SIGNAL_DISCARDED

def dispatch_key(key,down):
    global caret_handler
    if(caret_handler == None):
        dispatch_root_keybindings(key,down)
        if(mouse_focused_area is not None):
            area.rcv_key(key,down)
            return SIGNAL_HANDLED
    return caret_handler.rcv_key(key,down)

def dispatch_root_keybindings(key,down):
    pass

def caret_release():
    global caret_handler
    caret_handler = None
