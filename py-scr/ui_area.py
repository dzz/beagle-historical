import style
import gfx

areas = []

class mod_empty(object):
    def __init__(self):
        pass

    def rcv_mousemotion(self,ui_area,x,y):
        return False

    def rcv_mouse_button(self,ui_area,button,x,y,down):
        return False

    def rcv_key(self,ui_area,button,x,y,down):
        return False

    def transform_client_area(self,r):
        return r


class mod_titlebar(mod_empty):
    def __init__(self,ui_area,titlebar_text,height):
        super(self)
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
            return False 
        else:
            if( y < self.height ):
                self.toggled = True
                self.move_origin = [x,y]
                return True

    def rcv_mousemotion(self,ui_area,x,y):
        if self.toggled == False:
            return False
        else:
            ui_area.r[0] += x - self.move_origin[0]
            ui_area.r[1] += y - self.move_origin[1]
            self.move_origin = [x,y]

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
        self.renderer = None

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
        global areas
        for area in areas:
            area.z +=1
        self.z = 0
        order_areas()

class ui_window(ui_area):
    def __init__(self,title):
        super(self)
        self.modifier_stack = [mod_resize   ( self, resize_border = 2 ),
                               mod_titlebar ( self, titlebar_text = title, height = 8 ) ]

        self.renderers = window_renderer(self)

class renderer(object):
    def render(ui_area):
        pass

class window_renderer(renderer):
    def __init__(self,ui_area):
        self.label = gfx.label( text        = self.prop["titlebar_text"], 
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
                         self.bg_col) 
        self.label.draw(ui_area.r)

#controller 
def register_ui_area(area):
    global areas
    areas.append(area)

def remove_ui_area(area):
    global areas
    areas.remove(area)

#access
def order_areas():
    global areas
    areas.sort( key=lambda x: x.z, reverse = True )
    return areas

def get_ui_areas():
    return areas

def find_ui_area(x,y):
    for area in order_areas():
        if( x >= area.r[0] and x < area.r[0] + area.r[2] and
                y >= area.r[1] and y < area.r[1] + area.r[3] ):
                    return area
    return ui_area()
