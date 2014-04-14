import style
import gfx
import hwgfx

areas = []
__mpos = [0,0]

def set_absolute_mpos(mpos):
    global __mpos
    __mpos = mpos

def _glbl_mpos():
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

class mod_empty(object):
    def __init__(self):
        pass

    def rcv_mousemotion(self,ui_area,x,y):
        return SIGNAL_CONTINUE_HANDLING

    def rcv_mouse_button(self,ui_area,button,x,y,down):
        return SIGNAL_CONTINUE_HANDLING

    def rcv_key(self,ui_area,key,down):
        return SIGNAL_CONTINUE_HANDLING

    def transform_client_area(self,r):
        return r

class mod_titlebar(mod_empty):
    def __init__(self,ui_area,titlebar_text,height):
        self.cursor_origin = [0,0]
        self.origin = [0,0]
        self.height = height
        self.toggled = False
        ui_area.prop["titlebar_text"] = titlebar_text
        ui_area.prop["titlebar_height"] = height

    def transform_client_area(self,r):
        r[3] -= self.height
        r[1] += self.height
        return r

    def rcv_mouse_button(self,ui_area,button,x,y,down):
        if down == False:
            self.toggled = False
            return SIGNAL_CONTINUE_HANDLING 
        else:
            if( y < self.height ):
                self.toggled = True
                self.origin = [ ui_area.r[0], ui_area.r[1] ]
                mpos = _glbl_mpos()
                self.cursor_origin = [ mpos[0], mpos[1] ]
                return SIGNAL_EXIT_HANDLER

    def rcv_mousemotion(self,ui_area,x,y):
        if self.toggled == False:
            return SIGNAL_CONTINUE_HANDLING
        else:
            mpos = _glbl_mpos()
            ui_area.r[0] = self.origin[0] + mpos[0] - self.cursor_origin[0]
            ui_area.r[1] = self.origin[1] + mpos[1] - self.cursor_origin[1]
            self.move_origin = [x,y]
            return SIGNAL_EXIT_HANDLER

def _cascade_signal(ui_area,func,x,y,addtl_params):
        for child in ui_area.children:
            if(xy_in_r(x,y,child.r)):
                    x_t = x - ui_area.x
                    y_t =y - ui_area.y
                    child.func(x_t,y_t,*addtl_params)
                    return SIGNAL_EXIT_HANDLER
        return SIGNAL_CONTINUE_HANDLING

class mod_parent(mod_empty):
    def rcv_mousemotion(self,ui_area,x,y):
        return _cascade_signal(ui_area,ui_area.rcv_mouse_button, x,y,[])

    def rcv_mouse_button(self,ui_area,button,x,y,down):
        return _cascade_signal(ui_area,ui_area.rcv_mouse_button, x,y, [down])

    #def rcv_key(self,ui_area,key,down):
    #    return _cascade_signal(ui_area,ui_area.rcv_mouse_button, x,y, [key,down])


class mod_resize(mod_empty):
    def __init__(self, handle_size):
        self.toggled = False
        self.resize_origin = [0,0]
        self.handle_size = handle_size

    def transform_client_area(self,r):
        return r

    def rcv_mouse_button(self,ui_area,button,x,y,down):
        [ gx, gy ] = _glbl_mpos()
        if down == False:
            self.toggled = False
            return False
        else:
            self.resize_origin = [gx, gy]
            if( (x > ( ui_area.r[2] - self.handle_size )) and
                (y > ( ui_area.r[3] - self.handle_size )) ):
                self.toggled = True
                return SIGNAL_EXIT_HANDLER

            return SIGNAL_CONTINUE_HANDLING

    def rcv_mousemotion(self,ui_area,x,y):
        [ x, y ] = _glbl_mpos()
        if self.toggled:
            ui_area.r[2] += x - self.resize_origin[0]
            ui_area.r[3] += y - self.resize_origin[1]
            ui_area.r[2] = max(ui_area.r[2],self.handle_size)
            ui_area.r[3] = max(ui_area.r[3],self.handle_size)
            self.resize_origin = [x,y]
            return SIGNAL_EXIT_HANDLER

        return SIGNAL_CONTINUE_HANDLING

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

class ui_window(ui_area):
    def __init__(self,title="ctt2_window",x=200,y=200,width=200,height=200):
        ui_area.__init__(self)
        self.r = [x,y,width,height]
        self.modifier_stack = [
                               mod_resize   ( handle_size = 9 ),
                               mod_titlebar ( self, titlebar_text = title, height = 12 ),
                               mod_parent   ( )]

        self.renderers = [ window_renderer(self) ]

class renderer(object):
    def render(ui_area):
        pass

class window_renderer(renderer):
    def __init__(self,ui_area):
        self.label = gfx.label( text        = ui_area.prop["titlebar_text"], 
                                color       = style.get("window_titlebar_text_color"),
                                fontsize    = style.get("window_titlebar_fontsize"))

        self.title_bgcol    = style.get("window_titlebar_bg_color")
        self.bgcol          = style.get("window_bg_color")
        self.clientcol      = style.get("window_client_color")

    def render(self, ui_area):
        gfx.solid_rect( ui_area.r, self.bgcol)
        gfx.solid_rect( [ui_area.r[0],
                         ui_area.r[1],
                         ui_area.r[2], 
                         ui_area.prop.get("titlebar_height")],
                         self.title_bgcol) 
        self.label.draw(ui_area.r)
        gfx.solid_rect( ui_area.compute_client_area(), self.clientcol)

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

#from api
def init_ui_areas():
    window = ui_window()
    window2 = ui_window()
    #sub_window = ui_window()
    #window.add_child(sub_window)
    register_ui_area(window)
    register_ui_area(window2)

