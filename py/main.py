import caret
import ui_area

mouse_focused_area = None
caret_handler = None

def tick():
    render()
    pass

def render():
    pass

def dispatch_mouseup(button,x,y):
    mouse_focused_area.rcv_mouse_button(button,x,y, down = False)
    mouse_focused_area = None
    pass

def dispatch_mousedown(button,x,y):
    area = find_ui_area(x,y)
    if area is not None:
        area.rcv_mouse_button(button,x,y, down = True)

def dispatch_mousemotion(x,y):
    if(mouse_focused_area is not None):
        area = find_ui_area(x,y)
        m_pos = _calculate_mouse_position(area,x,y)
        area.set_m_pos(m_pos)

def dispatch_key(key,down):
    if(caret_handler == None):
        dispatch_root_keybindings(button,x,y,down)
        return
    caret_handler.rcv_key(key,down)
