import hwgfx

# mock gfx api methods
def hwgfx_genlabel(text,color,fontsize):
    return [0,0,0]

def hwgfx_draw_texquad( x, y, w, h ):
    pass

def hwgfx_draw_solidquad(x, y, w, h, r, g, b):
    pass

def hwgfx_draw_solidtri(x1,y1,x2,y2,x3,y3,r,g,b):
    pass

def hwgfx_bind_texture(tex_id):
    pass
#

class label(object):
    def __init__(self, text, color, fontsize ):
        self._label = hwgfx.label_generate()
        hwgfx.debug_displaykill()
        print "%s" % self._label;
        hwgfx.label_set_text(self._label,text)

    def __del__(self):
        hwgfx_drop_label(self._label)

    def set_text(text):
        pass

    def draw(self,x,y):
        pass

def solid_rect(r,color):
    hwgfx.draw_solidquad( r[0],r[1],r[2],r[3],
               color[0],color[1],color[2] )
