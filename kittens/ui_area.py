import style
import gfx
import hwgfx

areas = []
__mpos = [0,0]

def set_absolute_mpos(mpos):
    global __mpos
    __mpos = mpos

def glbl_mpos():
    global __mpos
    return __mpos

def _zsort(has_z):
    has_z.sort( key=lambda x: x.z, reverse = True )
    return has_z

def xy_in_r(x,y,r):
    return (x>=r[0] 
            and y>=r[1] 
            and (x < r[0]+r[2]) 
            and (y < r[1]+r[3]))

SIGNAL_EXIT_HANDLER = True
SIGNAL_CONTINUE_HANDLING = False

class ui_area(object):
    def __init__(self):
        self.r = [0,0,0,0]
        self.client_area = [0,0,0,0]
        self.z = 0;
        self.m = [0,0]
        self.active = False # set in main.py dispatch
        self.modifier_stack = []
        self.prop = {}
        self.renderers = []
        self.children = []

    def add_child(self,ui_area):
        self.children.append(ui_area)

    def compute_client_area(self):
        self.client_area = list(self.r)
        for modifier in self.modifier_stack:
            self.client_area = modifier.transform_client_area(self.client_area)
        return self.client_area

    def rcv_mouse_button(self,button,x,y,down):
        self.bring_top()
        for modifier in self.modifier_stack:
            if modifier.rcv_mouse_button(self,button,x,y,down) == SIGNAL_EXIT_HANDLER:
                return

    def rcv_mousemotion(self,x,y):
        for modifier in self.modifier_stack:
            if modifier.rcv_mousemotion(self,x,y) == SIGNAL_EXIT_HANDLER:
                return
            
    def rcv_key(self,key,down):
        for modifier in self.modifier_stack:
            if modifier.rcv_key(self,key,down) == SIGNAL_EXIT_HANDLER:
                return

    def set_m(self,position):
        self.m_pos = position
        self.rcv_mousemotion(self.m_pos[0],self.m_pos[1])

    def bring_top(self):
        for area in get_ui_areas():
            area.z +=1
        self.z = 0
        order_areas()



#controller 
def register_ui_area(area):
    get_ui_areas().append(area)

def remove_ui_area(area):
    get_ui_areas().remove(area)

def order_areas():
    return _zsort(get_ui_areas())

def get_ui_areas():
    global areas
    return areas

def find_ui_area(x,y):
    for area in order_areas():
        if( x >= area.r[0] and x < area.r[0] + area.r[2] and
                y >= area.r[1] and y < area.r[1] + area.r[3] ):
                    return area
    return None
