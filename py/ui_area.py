areas = []

class ui_area(object):
    def __init__(self):
        self.r = [0,0,0,0]
        self.z = 0;
        self.m_pos = (0,0)

    def rcv_mouse_button(button,x,y,down):
        pass

    def rcv_key(key,down):
        pass

    def set_m_pos(position):
        self.m_pos = position

def register_ui_area(area):
    areas.append(area)

def remove_ui_area(area):
    areas.remove(area)

def get_ui_areas():
    return areas

def find_ui_area(x,y):
    global areas
    areas.sort( key=lambda x: x.z, reverse = True )

    for area in areas:
        if( x >= area.r[0] and x < area.r[0] + area.r[2] and
                y >= area.r[1] and y < area.r[1] + area.r[3] ):
                    return area
    return None
