import style
import gfx

areas = []

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
        self.move_origin = [0,0]
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
                self.move_origin = [x,y]
                return SIGNAL_EXIT_HANDLER

    def rcv_mousemotion(self,ui_area,x,y):
        if self.toggled == False:
            return False
        else:
            ui_area.r[0] += x - self.move_origin[0]
            ui_area.r[1] += y - self.move_origin[1]
            self.move_origin = [x,y]

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
        return _cascade_signal(ui_area,ui_area.rcv_mouse_button, [x,y])

    def rcv_mouse_button(self,ui_area,button,x,y,down):
        return _cascade_signal(ui_area,ui_area.rcv_mouse_button, [x,y, down])

    def rcv_key(self,ui_area,key,down):
        return _cascade_signal(ui_area,ui_area.rcv_mouse_button, [key,down])


class mod_resize(mod_empty):
    def __init__(self, resize_border):
        self.toggled = False
        self.resize_border = resize_border
        self.adjusting_border = None;
        self.resize_origin = [0,0]

    def transform_client_area(self,r):
        r[0] += self.resize_border
        r[1] += self.resize_border
        r[2] -= (self.resize_border*2)
        r[3] -= (self.resize_border*2)
        return r


    def rcv_mouse_button(self,ui_area,button,x,y,down):
        if down == False:
            self.toggled = False
            return False
        else:
            self.resize_origin = [x, y]
            if ( x - ui_area.r[0] < self.resize_border ):
                self.adjusting_border = 0
                self.toggled = True
                return
            if ( ui_area.r[2] - x < self.resize_border ):
                self.adjusting_border = 1
                self.toggled = True
                return
            if ( y - ui_area.r[1] < self.resize_border ):
                self.adjusting_border = 2
                self.toggled = True
                return
            if ( ui_area.r[3] - y < self.resize_border ):
                self.adjusting_border = 3
                self.toggled = True
                return
            return True

    def rcv_mousemotion(self,ui_area,x,y):
        if self.toggled:
            if(self.adjusting_border < 2 ):
                ui_area.r[self.adjusting_border] += x - self.resize_origin[0]
            else:
                ui_area.r[self.adjusting_border] += y - self.resize_origin[1]
            self.resize_origin[x,y]
            return True
        return False

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

    def compute_client_area(self):
        self.client_area = self.r
        for modifier in modifier_stack:
            self.client_area = modifier.transform_client_area(self.client_area)
        return self.compute_client_area

    def rcv_mouse_button(self,button,x,y,down):
        for modifier in modifier_stack:
            if modifier.rcv_mouse_button(self,button,x,y,down):
                return
            
    def rcv_key(self,key,down):
        for modifier in modifier_stack:
            if modifier.rcv_key(self,key,down):
                return

    def set_m(self,position):
        self.m_pos = position

    def bring_top(self):
        for area in get_ui_areas():
            area.z +=1
        self.z = 0
        order_areas()

class ui_window(ui_area):
    def __init__(self,title="ctt2_window",x=0,y=0,width=100,height=100):
        ui_area.__init__(self)
        self.r = [x,y,width,height]
        self.modifier_stack = [mod_resize   ( resize_border = 2 ),
                               mod_titlebar ( self, titlebar_text = title, height = 8 ),
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

    def render(self, ui_area):
        gfx.solid_rect( ui_area.r, self.bgcol)
        gfx.solid_rect( [ui_area.r[0],
                         ui_area.r[1],
                         ui_area.r[2], 
                         ui_area.prop.get("titlebar_height")],
                         self.bgcol) 
        self.label.draw(ui_area.r)

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
    register_ui_area(window)

